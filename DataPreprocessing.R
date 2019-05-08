# this is building for the training set and test set, thefore we must merge this
# and get a large matrix or dataframe so that we can train the machine learning
# model.
CalculateIndicators <- function(MinData, TickData, intraday, withDateNTime = TRUE) {
  data_min_total <- data.frame()
  for (key in intraday) {
    Date <- rep(as.numeric(key), length(data_min[[key]]$Time))
    data_min_total <- rbind(data_min_total, cbind(Date, data_min[[key]]))
  }
  
  hlc <- data_min_total[,c("High", "Low", "Close")]
  hl <- data_min_total[,c("High", "Low")]
  open <- data_min_total$Open
  high <- data_min_total$High
  low <- data_min_total$Low
  close <- data_min_total$Close
  vol <- data_min_total$Volume
  
  library(TTR)
  indices_total <- data.frame(
    atr = ATR(hlc),
    macd = MACD(close),
    mfi = MFI(hlc, vol),
    obv = OBV(close, vol),
    cmo = CMO(close),
    cmf = CMF(hlc, vol),
    momentum = momentum(close),
    bbands = BBands(hlc),
    rsi = RSI(close),
    percentK = stoch(hlc),
    wpR = WPR(hlc),
    roc_close = ROC(close),
    roc_vol = ROC(vol),
    chai_vol = chaikinVolatility(data_min_total[,c("High", "Low")])
  )
  
  pvt <- rep(0, length(close))
  pvt[1] = vol[1]
  for (i in 2:length(pvt)) {
    pvt[i] = pvt[i - 1] + vol[i] * (close[i] - close[i - 1]) / close[i - 1]
  }
  
  # 现在要计算的是能体现流动性的指标
  # amihud measure
  amihud_measure <- c(NA, diff(log(data_min_total$Close))) / data_min_total$Volume
  
  # spread and order imbalance and VPIN
  ofi <- c()
  for (key in intraday) {
    tickData <- data_tick[[key]]
    Spread <- tickData$Ask - tickData$Bid
    Bid_diff <- diff(tickData$Bid)
    Ask_diff <- diff(tickData$Ask)
    BidSize_diff <- ifelse(Bid_diff == 0, diff(tickData$BidSize), 
                           ifelse(Bid_diff > 0, tickData$BidSize, c(0, tickData$BidSize)))
    AskSize_diff <- ifelse(Ask_diff == 0, diff(tickData$AskSize), 
                           ifelse(Ask_diff > 0, tickData$AskSize, c(0, tickData$AskSize)))
    
    ofi_tick <- c(NA, BidSize_diff - AskSize_diff)
    tickData <- cbind(tickData, data.frame(Spread, ofi_tick))
    for (i in 1:length(data_min[[key]]$Time)) {
      ofi <- c(ofi, sum(na.omit(tickData$ofi_tick[data_min[[key]]$Time[i] / 100 == as.integer(tickData$Time / 100)])))
    }
  }
  
  # lix
  lix <- ifelse(data_min_total$High - data_min_total$Low ==0, 0,
                log10((data_min_total$Volume * data_min_total$Close) / (data_min_total$High - data_min_total$Low)))
  
  liq_total <- data.frame(pvt, amihud_measure, ofi, lix)
  feature <- cbind(data_min_total, scale(indices_total), scale(liq_total))
  if(withDateNTime == FALSE) {
    feature <- subset(feature, select = -(1:2))
  }
    
  return(feature)
}

LabelSample <- function(data, threshold, method = "return", params) {
  if (method == "return") {
    library(TTR)
    lag = params[1]
    value <- c(na.omit(SMA(c(NA, diff(log(data))), n = lag)), rep(NA, lag))
    label <- as.factor(ifelse(value > threshold, 1, ifelse(value < -threshold, -1, 0)))
    result <- data.frame(value, label)
  } else if (method == "trend.mean") {
    library(TTR)
    lag = params[1]
    value <- c(SMA(data, n= lag)[-(1:lag)], rep(NA, lag)) - data
    label <- as.factor(ifelse(value > threshold, 1, ifelse(value < -threshold, -1, 0)))
    result <- data.frame(value, label)
  }
  
  return(label)
}

FeatureExtration <- function(minData, tickData, intraday, dataToLabel, label_control, withDateNTime = TRUE) {
  feature <- CalculateIndicators(minData, tickData, intraday)
  labelData <- feature[,dataToLabel]
  threshold <- label_control$threshold
  labelMethod <- label_control$method
  params <- label_control$params
  label <- LabelSample(labelData, threshold, labelMethod, params)
  feature <- cbind(feature, label)
}
