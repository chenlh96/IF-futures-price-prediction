setwd("C:/Users/tony/Desktop/DDFinalPaperCode")
load("C:/Users/tony/Desktop/DDFinalPaperCode/.RData")
save.image(paste("C:/Users/tony/Desktop/DDFinalPaperCode", ".RData", sep = '/'))

TickFilePath = "C:/Users/tony/Desktop/DDFinalPaperCode/Data/KadinaIFTick"
TickFileList <- list.files(path = TickFilePath)

for (i in 1:length(TickFileList)) {
  TickData <- read.csv(paste(TickFilePath, TickFileList[i], sep = '/'))
  TickData <- TickData[-1,]
  TickData$BidSize <- TickData$BidSize / 100
  TickData$AskSize <- TickData$AskSize / 100
  write.csv(TickData, file = paste("C:/Users/tony/Desktop/DDFinalPaperCode/Data/IFTick", sub("KadinaIndicator", "IFTIckData", TickFileList[i]), sep = '/'), row.names = FALSE)
}

OneMinFilePath = "C:/Users/tony/Desktop/DDFinalPaperCode/Data/KadinaIFOneMin"
OneMinFileList <- list.files(path = OneMinFilePath)

for (i in 1:length(OneMinFileList)) {
  MinData <- read.csv(paste(OneMinFilePath, OneMinFileList[i], sep = '/'))
  MinData <- MinData[-1,]
  write.csv(MinData, file = paste("C:/Users/tony/Desktop/DDFinalPaperCode/Data/IFOneMin", sub("KadinaIndicator", "IFOneMinData", OneMinFileList[i]), sep = '/'), row.names = FALSE)
}

rm(TickFilePath, TickFileList, OneMinFilePath, OneMinFileList, TickData, MinData, i)
