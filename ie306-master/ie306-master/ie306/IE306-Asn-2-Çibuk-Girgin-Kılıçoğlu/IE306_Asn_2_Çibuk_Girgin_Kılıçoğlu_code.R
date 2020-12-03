library("readxl")
data <- read_excel("./Assignment2-Interarrival_Data-S2020.xls")
day1 <- as.numeric(unlist(data["Day 1"]))
day2 <- as.numeric(unlist(data["Day 2"]))
interarrival_days <- c(day1,day2)
intarv_sorted <- sort(interarrival_days)

#Q1
kolmogorov_smirnoff_uniform <- function(data,a,b){# Uniform range [a,b]
  data <- (data-a)/b # Convert values to uniform 0-1 interval 
  i <- 1:length(data); 
  i_1_n <- (i-1)/length(i); # Calculate (i-1)/n
  i_n <- i/length(i); # Calculate i/n
  return(max(max(i_n - data),max( data - i_1_n))) 
}
d <- kolmogorov_smirnoff_uniform(intarv_sorted,0,400) # Calculating D value
if(d >= 1.36/sqrt(length(intarv_sorted))){# If D is greater then or equal to D_alpha_N then reject
  print("Reject: uniform in range 0-400")
}else{
  print("Accept: uniform in range 0-400")
}
#Q2
getmode <- function(v) {
  uniqv <- unique(v)
  uniqv[which.max(tabulate(match(v, uniqv)))]
}
data_mean <- mean(interarrival_days)
data_sd <- sd(interarrival_days)
data_max <- max(interarrival_days)
data_min <- min(interarrival_days)
data_range <- data_max -data_min
data_median <- median(interarrival_days)
data_mode <- getmode(interarrival_days)
sprintf("Mean: %s", data_mean)
sprintf("Standart deviation: %s", data_sd)
sprintf("Max: %s", data_max)
sprintf("Min: %s", data_min)
sprintf("Range: %s", data_range)
sprintf("Median: %s", data_median)
sprintf("Mode: %s", data_mode)

#Q3
# Histograms for different time intervals
hist(interarrival_days, breaks=as.integer(data_max/20))
hist(interarrival_days, breaks=as.integer(data_max/10))
hist(interarrival_days, breaks=as.integer(data_max/5))
#Q4
chisquare_test_exponential <- function(observed, bins){
  rate = 1/mean(observed)
  chisquare = 0
  for(k in 1:bins){
    o_i = sum((observed <= k*10) & (observed >= k*10-10)) #Observed in the bin
    e_i = length(observed)*(pexp(k*10,rate = rate)-pexp(k*10-10,rate = rate)) # Expected in bin
    chisquare = chisquare + (o_i-e_i)^2 /e_i
  }
  return(chisquare)
}
chi2val <- chisquare_test_exponential(interarrival_days, 44)
alpha_found <- pchisq(chi2val, df=44-1-1, lower.tail=FALSE)
alpha_desired <- 0.05
if(alpha_found <= alpha_desired){
  sprintf("Reject: exponential with mean %s", data_mean)
}else{
  sprintf("Accept: exponential with mean %s", data_mean)
}

#Q5
p <- ppoints(100)  # 100 equally spaced points  
q <- quantile(interarrival_days,p=p) # percentiles of the sample distribution
plot(qexp(p) ,q, main="Exponential Q-Q Plot", xlab="Theoretical Quantiles",ylab="Sample Quantiles") # data
qqline(q, distribution=qexp,col="blue", lty=2) # qqline

#Q6
obs_times <- cumsum(data) # find cumulative sum of up to each index
day1_obs <- as.numeric(unlist(obs_times["Day 1"]))
day2_obs <- as.numeric(unlist(obs_times["Day 2"]))

plot(day1_obs, day1, type="l", col="blue", main="Day 1", xlab = "Observation Time", ylab = "Interarrival Times")
plot(day2_obs, day2, type="l", col="red", main="Day 2", xlab = "Observation Time", ylab = "Interarrival Times")

#Q7
# 1 and 2 lag autocorrelation test for day1
auto_corr_d1 = acf(day1, lag.max = 2, plot=FALSE) 

plot(auto_corr_d1[1:2])
print(auto_corr_d1[1:2])

# 1 and 2 lag autocorrelation test for day2
auto_corr_d2 = acf(day2, lag.max = 2, plot=FALSE)

plot(auto_corr_d2[1:2])
print(auto_corr_d2[1:2])

# Lag1 diff
day1_lag1_diff <- diff(day1,lag=1)
day2_lag1_diff <- diff(day2,lag=1)
plot(day1_lag1_diff)
plot(day2_lag1_diff)
# Lag2 diff
day1_lag2_diff <- diff(day1,lag=2)
day2_lag2_diff <- diff(day2,lag=2)
plot(day1_lag2_diff)
plot(day2_lag2_diff)


