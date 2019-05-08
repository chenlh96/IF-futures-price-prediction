ReadContract <- function(path, key) {
  file_path <- path
  file_list <- list.files(path = file_path)
  start = key[1]
  end = key[2]
  data <- list()
  
  for (f in file_list) {
    key_pos <- substr(f, start, end)
    data[[key_pos]] <- read.csv(paste(file_path, f, sep = '/'))
  }
  
  return(data)
}

