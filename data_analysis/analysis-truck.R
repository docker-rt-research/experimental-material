options(warn=-1)
require(heplots)

cat("############### ---------------- ##################\n")
cat("###############   Reading data   ##################\n")
cat("############### ---------------- ##################\n")
data <- read.table("[INSERT LOCATION OF PARSED TRUCK DATA]",sep=";", header=T);


keeps <- c("odv_oh1", "pi_calc", "odv_oh2", "sleep")

cat("############### ---------------- ##################\n")
cat("###############     mean, sd     ##################\n")
cat("############### ---------------- ##################\n")
print(sapply(data[keeps], mean))
print(sapply(data[keeps], sd))



cat("\n\n")
cat("############### Sorting the data ##################\n")
native	<-data[(data$deployment	== "0"),]
docker	<-data[(data$deployment	== "1"),]

load_scen_1 <- data[(data$deployment	== "0" & data$kernel	== "1"),]
load_scen_2 <- data[(data$deployment	== "1" & data$kernel	== "1"),]


cat("\n\n")
cat("############### ---------------- ##################\n")
cat("###############      MANOVA      ##################\n")
cat("############### ---------------- ##################\n")

Y <- cbind(data$odv_oh1,data$pi_calc,data$odv_oh2,data$sleep)
man <- manova(Y ~ data$deployment)
print(summary(man))
print(etasq(man))
print(man$coefficients)

cat("\n\n")
cat("############### ---------------- ##################\n")
cat("###############       DESCR      ##################\n")
cat("############### ---------------- ##################\n")
cat("\n\n")
cat("############### ---------------- ##################\n")
cat("###############      Native      ##################\n")
cat("############### ---------------- ##################\n")
print(sapply(native[keeps], mean))
print(sapply(native[keeps], sd))

cat("\n\n")
cat("############### ---------------- ##################\n")
cat("###############      Docker      ##################\n")
cat("############### ---------------- ##################\n")
print(sapply(docker[keeps], mean))
print(sapply(docker[keeps], sd))

cat("\n\n")
cat("############### ---------------- ##################\n")
cat("########### SUM COLUMNS FOR SCENARIOS #############\n")
cat("############### ------Load------ ##################\n")

cat("\n")
cat("###############    Scenario 1    ##################\n")
print(colSums(load_scen_1))
cat("\n")
cat("###############    Scenario 2    ##################\n")
print(colSums(load_scen_2))


cat("\n\n")
cat("############### ---------------- ##################\n")
cat("########### STD COLUMNS FOR SCENARIOS #############\n")
cat("############### ------Load------ ##################\n")


cat("\n")
cat("###############    Scenario 1     ##################\n")
print(sd(load_scen_1$odv_oh1))
print(sd(load_scen_1$pi_calc))
print(sd(load_scen_1$odv_oh2))
print(sd(load_scen_1$sleep))
cat("\n")
cat("###############    Scenario 2     ##################\n")
print(sd(load_scen_2$odv_oh1))
print(sd(load_scen_2$pi_calc))
print(sd(load_scen_2$odv_oh2))
print(sd(load_scen_2$sleep))


options(warn=0)