CalculateAccuracy <- function(data) {
  accuracy <- matrix(nrow = length(data), ncol = 3, 
                     dimnames = list(names(data), c("Overall", "UpDownOnly", "UpDnReverse")))
  for (i in 1:length(data)) {
    accuracy[i, 3] <- sum(data[[i]][1,3] + data[[i]][3, 1])/sum(data[[i]][,c(1,3)])
    accuracy[i, 2] <- sum(data[[i]][1,1] + data[[i]][3, 3])/sum(data[[i]][,c(1,3)])
    accuracy[i, 1] <- sum(diag(data[[i]]))/sum(data[[i]])
  }
  return(accuracy)
}

GetSetConfution <- function(feature_label, sepName, labelName, modelType, intraday_training_model, model_machineLearning, var_select) {
  conf_test <- list()
  conf_training <- list()
  len <- length(intraday_training_model)
  if (modelType == "logit") {
    library(nnet)
    for (i in 1:len) {
    set_test <- na.omit(subset(feature_label, feature_label[,sepName] >= intraday_training_model[i] & feature_label[,sepName] < ifelse(i == len, 20150901, intraday_training_model[i + 1]), select = c(var_select[[i]] + 2, length(feature_label))))
    set_training <- na.omit(subset(feature_label, feature_label[,sepName] < intraday_training_model[i], select = c(var_select[[i]] + 2, length(feature_label))))
    
    conf_test[[as.character(intraday_training_model[i])]] <- table(predict(model_machineLearning[[i]], set_test), set_test[,labelName])
    conf_training[[as.character(intraday_training_model[i])]] <- table(predict(model_machineLearning[[i]], set_training), set_training[,labelName])
    }
  } else if (modelType == "rmf") {
    library(randomForest)
    for (i in 1:len) {
      set_test <- na.omit(subset(feature_label, feature_label[,sepName] >= intraday_training_model[i] & feature_label[,sepName] < ifelse(i == len, 20150901, intraday_training_model[i + 1]), select = -(1:2)))
      set_test <- set_test[c(var_select[[i]], length(set_test))]
      
      set_training <- na.omit(subset(feature_label, feature_label[,sepName] < intraday_training_model[i], select = -(1:2)))
      set_training <- set_training[c(var_select[[i]], length(set_training))]
      
      conf_test[[as.character(intraday_training_model[i])]] <- table(predict(model_machineLearning[[i]], set_test), set_test[,labelName])
      conf_training[[as.character(intraday_training_model[i])]] <- table(predict(model_machineLearning[[i]], set_training), set_training[,labelName])
      
    }
  }
  
  pred <- list(training = list(conf_mat = conf_training, accuracy = CalculateAccuracy(conf_training)), 
               test = list(conf_mat = conf_test, accuracy = CalculateAccuracy(conf_test)))
  print(pred)
  return(pred)
}

GetBalancedData <- function(data, strataname = "label", med = "srswor", diff = 3000) {
  num <- table(data[,strataname])
  print(num)
  min = min(num)
  for (i in 1:length(num)) {
    if (abs(num[i] - min) > diff) { num[i] = min + round(runif(1,1,diff)) }
  }
  
  sampledata <- sampling::strata(data = data, stratanames = strataname, size = num, method = med)
  balanceData <- data[sampledata$ID_unit,]
  print(table(balanceData[,strataname]))
  return(balanceData)
}