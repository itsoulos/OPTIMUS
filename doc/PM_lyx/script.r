library(tidyverse)
library(readxl)
library(ggpubr)
library(scales)
library(ggplot2)

dat <- read_excel("C:/Users/admin/Desktop/PM/PM_lyx/pm.dataset3.xlsx")
df <- dat |>
  rename(problem = 1) |>
  pivot_longer(-problem) 

cmp <- list(c("PROPOSED", "DOUBLEBOX"), c("PROPOSED", "MEAN"), c("PROPOSED", "BEST"))
ggboxplot(df, x = "name", y = "value", color = "name", shape = "name", add = "jitter")  +
  stat_compare_means(paired = TRUE, method = "t.test", comparisons = cmp) +
  stat_compare_means(paired = TRUE, label.y = 80000) +
  labs(x = "Termination rule", y = "Function calls", shape = "", color = "", title = "", subtitle = "")+
  scale_y_continuous(labels = comma)
