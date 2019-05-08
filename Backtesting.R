Backtest <- function(data, signal, params) {
  n <- length(data[,1])
  buyprice = 0
  sellprice = 0
  pos = 0
  list_time <- rep(0, n)
  list_pnl <- rep(0, n)
  list_side <- rep(0, n)
  list_price <- rep(0, n)
  list_pos <- rep(0, n)
  list_money <- rep(0,n)
  list_break <- rep(0,n)
  
  currentmoney = params$money
  deposit = params$deposit
  slip = params$slipPoint
  exit = FALSE
  exitPoint = params$exitPoint
  fee <- params$feePerTrade
  totalfee <- 0
  
  for (i in 1:(n - 1)) {
    if (sum(list_pnl) < exitPoint) { exit = TRUE }
    
    if (is.na(signal[i]) == FALSE & exit == FALSE) {
      if (signal[i] == 1) {
        if (pos == 0) {
          buyprice = data$Close[i] + slip
          totalfee <- totalfee + buyprice * fee
          pos = 1
          currentmoney = currentmoney - buyprice * 300 * deposit
          
          list_time[i] = data$Time[i]
          list_price[i] = buyprice
          list_side[i] = 1
          list_pos[i] = pos
          list_money[i] = currentmoney
        } else if (pos == -1) {
          buyprice = data$Close[i] + slip
          pos = 0

          list_time[i] = data$Time[i]
          list_price[i] = buyprice
          list_side[i] = 1
          list_pos[i] = pos
          list_pnl[i] = sellprice - buyprice
          currentmoney = currentmoney + sellprice * 300 * deposit + list_pnl[i] * 300 
          list_money[i] = currentmoney
        }
      } else if (signal[i] == -1) {
        if (pos == 0) {
          sellprice = data$Close[i] - slip
          totalfee <- totalfee + sellprice * fee
          pos = -1
          currentmoney = currentmoney - sellprice * 300 * deposit
          
          list_time[i] = data$Time[i]
          list_price[i] = sellprice
          list_side[i] = -1
          list_pos[i] = pos
          list_money[i] = currentmoney
        } else if (pos == 1) {
          sellprice = data$Close[i] - slip
          pos = 0
          
          list_time[i] = data$Time[i]
          list_price[i] = sellprice
          list_side[i] = -1
          list_pos[i] = pos
          list_pnl[i] = sellprice - buyprice
          currentmoney = currentmoney + buyprice * 300 * deposit + list_pnl[i] * 300 
          list_money[i] = currentmoney
        }
      }
    }
  }
  
  if (pos == 1) {
    sellprice = data$Close[n] - slip
    totalfee <- totalfee + sellprice * fee
    pos = 0
    
    list_time[n] = data$Time[n]
    list_price[n] = sellprice
    list_side[n] = -1
    list_pos[n] = pos
    list_pnl[n] = sellprice - buyprice
    currentmoney = currentmoney + buyprice * 300 * deposit + list_pnl[n] * 300
    list_money[n] = currentmoney
  } else if (pos == -1) {
    buyprice = data$Close[n] + slip
    totalfee <- totalfee + sellprice * fee
    pos = 0
    
    list_time[n] = data$Time[n]
    list_price[n] = buyprice
    list_side[n] = 1
    list_pos[n] = pos
    list_pnl[n] = sellprice - buyprice
    currentmoney = currentmoney + sellprice * 300 * deposit + list_pnl[n] * 300
    list_money[n] = currentmoney
  } else if (pos == 0) { list_money[n] = currentmoney }
  
  result_trade <- data.frame(time = list_time, pnl = list_pnl, 
                       price = list_pnl, side = list_side,
                       pos = list_pos, money = list_money,
                       isbreak = list_break)
  return(result_trade)
}

TradeAnalysis <- function(data) {
  n <- length(data)
  winPerDayByLong <- rep(0, n)
  winPerDayByShort <- rep(0, n)
  lossPerDayByLong <- rep(0, n)
  lossPerDayByShort <- rep(0, n)
  winvolumeByLong <- rep(0, n)
  winvolumeByShort <- rep(0, n)
  lossvolumeByLong <- rep(0, n)
  lossvolumeByShort <- rep(0, n)
  maxddPerDay <- rep(0, n)
  sharp <- rep(0, n)
  maxdd = 0
  maxm = 0
  
  for (i in 1:n) {
    winPerDayByLong[i] = sum(data[[i]]$pnl[data[[i]]$pnl > 0 & data[[i]]$side == 1])
    winPerDayByShort[i] = sum(data[[i]]$pnl[data[[i]]$pnl > 0 & data[[i]]$side == -1])
    lossPerDayByLong[i] = sum(data[[i]]$pnl[data[[i]]$pnl < 0 & data[[i]]$side == 1])
    lossPerDayByShort[i] = sum(data[[i]]$pnl[data[[i]]$pnl < 0 & data[[i]]$side == -1])
    winvolumeByLong[i] <- sum(data[[i]]$side[data[[i]]$pnl > 0 & data[[i]]$side == 1]) * 2
    winvolumeByShort[i] <- -sum(data[[i]]$side[data[[i]]$pnl > 0 & data[[i]]$side == -1]) * 2
    lossvolumeByLong[i] <- sum(data[[i]]$side[data[[i]]$pnl < 0 & data[[i]]$side == 1]) * 2
    lossvolumeByShort[i] <- -sum(data[[i]]$side[data[[i]]$pnl < 0 & data[[i]]$side == -1]) * 2
    sharp[i] <- mean(data[[i]]$pnl) / sd(data[[i]]$pnl)
    
    maxdd = 0
    maxm = 0
    for (j in 1:length(data[[i]]$money)) {
      if (data[[i]]$money[j] != 0) {
        if (data[[i]]$money[j] > maxm) {maxm = data[[i]]$money[j] } 
        else if ((1 -  data[[i]]$money[j] / maxm) > maxdd) {
          maxdd = 1 -  data[[i]]$money[j] / maxm
        }
      }
    }
    maxddPerDay[i] = maxdd
  }
  
  winvolume <- winvolumeByLong + winvolumeByShort
  lossvolume <- lossvolumeByLong + lossvolumeByShort
  volume <- (winvolume + lossvolume)
  winshare <- winvolume / volume
  winPerDay <- winPerDayByLong + winPerDayByShort
  lossPerDay <- lossPerDayByShort + lossPerDayByLong
  pnlPerDay <- winPerDay + lossPerDay
  pnlCumulative <- cumsum(pnlPerDay)
  
  for (i in 1:n) {
    len = length(data[[i]]$money)
    if (data[[i]]$money[len] > maxm) { maxm = data[[i]]$money[len] } 
    else if ((1 -  data[[i]]$money[len] / maxm) > maxdd) {
      maxdd = 1 -  data[[i]]$money[len] / maxm
    }
  }
  
  indicator <- c(sum(pnlPerDay), sum(winPerDayByLong + lossPerDayByLong) , sum(winPerDayByShort + lossPerDayByShort),
                 mean(pnlPerDay), mean(winPerDayByLong + lossPerDayByLong), mean(winPerDayByShort + lossPerDayByShort),
                 mean(winPerDay), mean(winPerDayByLong), mean(winPerDayByShort),
                 mean(lossPerDay), mean(lossPerDayByLong), mean(lossPerDayByShort),
                 mean(winshare), mean(winvolumeByLong / volume), mean(winvolumeByShort / volume),
                 mean(volume), mean(winvolumeByLong + lossvolumeByLong), mean(winvolumeByShort + lossvolumeByShort),
                 mean(winvolume), mean(winvolumeByLong), mean(winvolumeByShort),
                 mean(lossvolume), mean(lossvolumeByLong), mean(lossvolumeByShort))
  singleIndicator = c(mean(sharp), mean(maxddPerDay), maxdd, length(pnlPerDay[pnlPerDay >= 0]), length(pnlPerDay[pnlPerDay < 0]))
  
  basicindices = matrix(indicator, nrow = 8, ncol = 3, byrow = TRUE, dimnames = list(c("totalpnl", "meanpnl", "meanwin", 
                        "meanloss", "winshare", "meanvol", "meanwinvol", "meanlossvol"), c("total", "Long", "Short")))
  riskindicies = matrix(singleIndicator, nrow = 1, ncol = length(singleIndicator), 
                        dimnames = list(c(""), c("SharpRatio", "meanmaxdd", "maxdd", "profit", "loss")))
  print(basicindices)
  print(riskindicies)
  plot(volume, type = "h")
  plot(pnlCumulative, type = "l")
  plot(pnlPerDay, type = "l")
  
  result <- list(pnlCumulative = pnlCumulative, pnlPerDay = pnlPerDay, volumePerDay = volume, winshare = winshare,
                 basicindices = basicindices, riskindicies = riskindicies)
  
  return(result)
}
