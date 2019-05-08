best <- list(
  varsel = model_varsel,
  pred = model_pred,
  tradelist = tradelist
)

plotClosePrice <- function(plot_parms, intraday, data, closePrice) {
  par(mfrow = plot_parms)
  for (date in intraday) {
    plot(data[[date]][,closePrice], type = "l", main = date)
  }
  
  par(mfrow = c(1, 1))
}
plotClosePrice(c(4, 2), intraday_test[103:120], data_min, "Close")


plot(intraday_test, trade_indicators$pnlCumulative)

library(ggplot2)
ggplot(mapping = aes(x  = as.Date.character(intraday_test, "%Y%m%d"), y = trade_indicators$pnlCumulative)) + 
  geom_line(size = 0.8, color = "blue", linetype = "solid") + geom_point(color = "blue ", shape = 17) + 
  labs(title = "策略累计收益", x = "交易日期", y = "当日累计收益/点")

ggplot(mapping = aes(x  = as.Date.character(intraday_test, "%Y%m%d"), y = trade_indicators$volumePerDay)) + 
  geom_bar(color = "gray",stat="identity") + labs(x = "交易日期", y = "交易手数", title = "每日交易手数")

ggplot(mapping = aes(x  = as.Date.character(intraday_test, "%Y%m%d"), y = trade_indicators$pnlPerDay)) + 
  geom_bar(color = "lightblue",stat="identity") + labs(x = "交易日期", y = "每日收益/点", title = "每日收益点数")

set_test_tmp <- na.omit(subset(feature_label, feature_label$Date >= intraday_test[103] & feature_label$Date < intraday_test[120], select = c(varimp[[3]] + 2, length(feature_label))))
table(predict(model_pred[[3]], set_test_tmp), set_test_tmp$label)
