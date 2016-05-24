options(warn=-1)
require(heplots)

cat("############### ---------------- ##################\n")
cat("###############   Reading data   ##################\n")
cat("############### ---------------- ##################\n")
data <- read.table("[INSERT LOCATION OF PARSED PIIO DATA]",sep=";", header=T);


keeps <- c("camio", "diskio")

cat("############### ---------------- ##################\n")
cat("############### Length, mean, sd ##################\n")
cat("############### ---------------- ##################\n")
print(length(data$diskio))
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
cat("########### SUM COLUMNS FOR SCENARIOS #############\n")
cat("############### ------Load------ ##################\n")

print(colSums(load_scen_1))
print(colSums(load_scen_2))
print(colSums(load_scen_3))
print(colSums(load_scen_4))


cat("\n\n")
cat("############### ---------------- ##################\n")
cat("########### SUM COLUMNS FOR SCENARIOS #############\n")
cat("############### -----No-Load---- ##################\n")

print(colSums(noload_scen_1))
print(colSums(noload_scen_2))
print(colSums(noload_scen_3))
print(colSums(noload_scen_4))

cat("\n\n")
cat("############### ---------------- ##################\n")
cat("########### STD COLUMNS FOR SCENARIOS #############\n")
cat("############### ------Load------ ##################\n")

print(sd(load_scen_1$camio))
print(sd(load_scen_1$diskio))
print(sd(load_scen_2$camio))
print(sd(load_scen_2$diskio))
print(sd(load_scen_3$camio))
print(sd(load_scen_3$diskio))
print(sd(load_scen_4$camio))
print(sd(load_scen_4$diskio))


cat("\n\n")
cat("############### ---------------- ##################\n")
cat("########### STD COLUMNS FOR SCENARIOS #############\n")
cat("############### -----No-Load---- ##################\n")

print(sd(noload_scen_1$camio))
print(sd(noload_scen_1$diskio))
print(sd(noload_scen_2$camio))
print(sd(noload_scen_2$diskio))
print(sd(noload_scen_3$camio))
print(sd(noload_scen_3$diskio))
print(sd(noload_scen_4$camio))
print(sd(noload_scen_4$diskio))


cat("\n\n")
cat("############### ---------------- ##################\n")
cat("############### CAMERADISK ANOVA ##################\n")
cat("############### ---------------- ##################\n")
anovacam <- aov(data$camio ~ data$deployment*data$kernel*data$load)
print(summary(anovacam))
print(etasq(anovacam))
print(anovacam$coefficients)


cat("\n\n")
cat("############### ---------------- ##################\n")
cat("###############   DISKIO ANOVA   ##################\n")
cat("############### ---------------- ##################\n")
anovadisk <- aov(data$diskio ~ data$deployment*data$kernel*data$load)
print(summary(anovadisk))
print(etasq(anovadisk))
print(anovadisk$coefficients)



cat("\n\n")
cat("############### ---------------- ##################\n")
cat("############### CAMERADISK DESCR ##################\n")
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



options(warn=0)