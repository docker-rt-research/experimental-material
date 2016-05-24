options(warn=-1)
require(heplots)

cat("############### ---------------- ##################\n")
cat("###############   Reading data   ##################\n")
cat("############### ---------------- ##################\n")
data <- read.table("[INSERT LOCATION OF PARSED PI DATA]",sep=";", header=T);


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
vanilla	<-data[(data$kernel		== "0"),]
rt 		<-data[(data$kernel		== "1"),]
noload	<-data[(data$load			== "0"),]
cpuload	<-data[(data$load			== "1"),]

load_scen_1 <- cpuload[(cpuload$deployment	== "0" & cpuload$kernel	== "0"),]
load_scen_2 <- cpuload[(cpuload$deployment	== "0" & cpuload$kernel	== "1"),]
load_scen_3 <- cpuload[(cpuload$deployment	== "1" & cpuload$kernel	== "0"),]
load_scen_4 <- cpuload[(cpuload$deployment	== "1" & cpuload$kernel	== "1"),]

noload_scen_1 <- noload[(noload$deployment	== "0" & noload$kernel	== "0"),]
noload_scen_2 <- noload[(noload$deployment	== "0" & noload$kernel	== "1"),]
noload_scen_3 <- noload[(noload$deployment	== "1" & noload$kernel	== "0"),]
noload_scen_4 <- noload[(noload$deployment	== "1" & noload$kernel	== "1"),]


cat("\n\n")
cat("############### ---------------- ##################\n")
cat("###############      MANOVA      ##################\n")
cat("############### ------Both------ ##################\n")

Y <- cbind(data$odv_oh1,data$pi_calc,data$odv_oh2,data$sleep)
man <- manova(Y ~ data$deployment + data$kernel + data$load + data$deployment:data$kernel:data$load)
print(summary(man))
print(etasq(man))
print(man$coefficients)

cat("\n\n")
cat("############### ---------------- ##################\n")
cat("###############      MANOVA      ##################\n")
cat("############### ------Load------ ##################\n")

Y <- cbind(cpuload$odv_oh1,cpuload$pi_calc,cpuload$odv_oh2,cpuload$sleep)
man <- manova(Y ~ cpuload$deployment + cpuload$kernel + cpuload$deployment:cpuload$kernel)
print(summary(man))
print(etasq(man))
print(man$coefficients)

cat("\n\n")
cat("############### ---------------- ##################\n")
cat("###############      MANOVA      ##################\n")
cat("############### ----No Load----- ##################\n")

Y <- cbind(noload$odv_oh1,noload$pi_calc,noload$odv_oh2,noload$sleep)
man <- manova(Y ~ noload$deployment + noload$kernel + noload$deployment:noload$kernel)
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
cat("###############      Vanilla     ##################\n")
cat("############### ---------------- ##################\n")
print(sapply(vanilla[keeps], mean))
print(sapply(vanilla[keeps], sd))

cat("\n\n")
cat("############### ---------------- ##################\n")
cat("###############        RT        ##################\n")
cat("############### ---------------- ##################\n")
print(sapply(rt[keeps], mean))
print(sapply(rt[keeps], sd))

cat("\n\n")
cat("############### ---------------- ##################\n")
cat("###############      No Load     ##################\n")
cat("############### ---------------- ##################\n")
print(sapply(noload[keeps], mean))
print(sapply(noload[keeps], sd))

cat("\n\n")
cat("############### ---------------- ##################\n")
cat("###############      CPULoad     ##################\n")
cat("############### ---------------- ##################\n")
print(sapply(cpuload[keeps], mean))
print(sapply(cpuload[keeps], sd))


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
cat("\n")
cat("###############    Scenario 3    ##################\n")
print(colSums(load_scen_3))
cat("\n")
cat("###############    Scenario 4    ##################\n")
print(colSums(load_scen_4))


cat("\n\n")
cat("############### ---------------- ##################\n")
cat("########### SUM COLUMNS FOR SCENARIOS #############\n")
cat("############### -----No-Load---- ##################\n")


cat("\n")
cat("###############    Scenario 1    ##################\n")
print(colSums(noload_scen_1))
cat("\n")
cat("###############    Scenario 2    ##################\n")
print(colSums(noload_scen_2))
cat("\n")
cat("###############    Scenario 3    ##################\n")
print(colSums(noload_scen_3))
cat("\n")
cat("###############    Scenario 4    ##################\n")
print(colSums(noload_scen_4))

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
cat("\n")
cat("###############    Scenario 3     ##################\n")
print(sd(load_scen_3$odv_oh1))
print(sd(load_scen_3$pi_calc))
print(sd(load_scen_3$odv_oh2))
print(sd(load_scen_3$sleep))
cat("\n")
cat("###############    Scenario 4     ##################\n")
print(sd(load_scen_4$odv_oh1))
print(sd(load_scen_4$pi_calc))
print(sd(load_scen_4$odv_oh2))
print(sd(load_scen_4$sleep))


cat("\n\n")
cat("############### ---------------- ##################\n")
cat("########### STD COLUMNS FOR SCENARIOS #############\n")
cat("############### -----No-Load---- ##################\n")


cat("\n")
cat("###############    Scenario 1     ##################\n")
print(sd(noload_scen_1$odv_oh1),)
print(sd(noload_scen_1$pi_calc))
print(sd(noload_scen_1$odv_oh2))
print(sd(noload_scen_1$sleep))
cat("\n")
cat("###############    Scenario 2     ##################\n")
print(sd(noload_scen_2$odv_oh1))
print(sd(noload_scen_2$pi_calc))
print(sd(noload_scen_2$odv_oh2))
print(sd(noload_scen_2$sleep))
cat("\n")
cat("###############    Scenario 3     ##################\n")
print(sd(noload_scen_3$odv_oh1))
print(sd(noload_scen_3$pi_calc))
print(sd(noload_scen_3$odv_oh2))
print(sd(noload_scen_3$sleep))
cat("\n")
cat("###############    Scenario 4     ##################\n")
print(sd(noload_scen_4$odv_oh1))
print(sd(noload_scen_4$pi_calc))
print(sd(noload_scen_4$odv_oh2))
print(sd(noload_scen_4$sleep))


options(warn=0)