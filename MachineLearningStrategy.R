library(varSelRF)
library(rpart)
library(nnet)


intraday_test_bound = c(20150105, 20150831)
intraday_training_model <- c(20150105, 20150302, 20150504, 20150703)
num_feature_sel = 15
params_label <- list(threshold = 0.6, method = "trend.mean", params = c(2))
params_backtest <- list(slipPoint = 0.25, exitPoint = -40, feePerTrade = 2.5 * 10e-3, money = 500000, deposit = 0.2)

data_min <- ReadContract("D:/CLHtmp/DDFinalPaperCode/Data/IFOneMin", key = c(13, 20))
data_tick <- ReadContract("D:/CLHtmp/DDFinalPaperCode/Data/IFTick", key = c(11, 18))
intraday <- names(data_min)
feature_label <- FeatureExtration(data_min, data_tick, intraday, "Close", label_control = params_label)

tradelist <- list()
model_pred <- list()
model_varsel <- list()
varimp <- list()
signal_probs <- list()

for (date in intraday[intraday >= intraday_test_bound[1] & intraday <= intraday_test_bound[2]]) {
  if (date == intraday_test_bound[1]) { params_backtest$money = 600000 }
  if (sum(date == intraday_training_model) == 1) {
    print(date)
    
    set_training<- GetBalancedData(na.omit(subset(feature_label, feature_label$Date < date, select = -(1:2))), strataname = "label", med = "srswor")
    #model_varsel[[date]] <- randomForest(label ~ ., data = set_training, ntree = 200, importance = TRUE)
    print(model_varsel[[date]])
    
    varImpPlot(model_varsel[[date]])
    varimp[[date]] <- order(importance(model_varsel[[date]], type = 2), decreasing = TRUE)[1:num_feature_sel]
    varimp_current <- varimp[[date]]
    
    set_training <- set_training[c(varimp_current, length(set_training))]
    #model_pred[[date]] <- multinom(label~., set_training)
    fit_logit <- model_pred[[date]]
  }
  
  feature_min <- feature_label[feature_label$Date == date, c(varimp_current + 2, length(feature_label))]
  signal_probs[[date]] <- cbind(predict(fit_logit, feature_min, type = "probs"), label = feature_min$label)
  signal<- predict(fit_logit, feature_min)
  tradelist[[date]] <- Backtest(data_min[[date]], signal, params_backtest)
  params_backtest$money = tradelist[[date]]$money[length(tradelist[[date]]$money)]
}

conf_mat <- GetSetConfution(feature_label, "Date", "label", "logit", intraday_training_model, model_pred, varimp)
trade_indicators <- TradeAnalysis(tradelist)
