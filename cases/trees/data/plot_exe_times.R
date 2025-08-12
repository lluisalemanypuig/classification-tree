require(patchwork)
require(ggplot2)
require(dplyr)

read.data <- function(n) {
	data0 = read.table(paste0("0/times_", n, ".txt"), header = T, sep = "\t")
	data11 = read.table(paste0("1_Dminpl/times_", n, ".txt"), header = T, sep = "\t")
	data12 = read.table(paste0("1_Cexp/times_", n, ".txt"), header = T, sep = "\t")
	data21 = read.table(paste0("2_Dminpl_Cexp/times_", n, ".txt"), header = T, sep = "\t")
	data22 = read.table(paste0("2_Dminpl_Cvar/times_", n, ".txt"), header = T, sep = "\t")
	data3 = read.table(paste0("3_Dminpl_Cexp_Cvar/times_", n, ".txt"), header = T, sep = "\t")
	
	return (rbind(data0, data11, data12, data21, data22, data3))
}

LABELS = c(
	`0` = "No features",
	`1_Dminpl` = "Min planar D",
	`1_Cexp` = "Expected C",
	`2_Dminpl_Cexp` = "Min planar D, Expected C",
	`2_Dminpl_Cvar` = "Min planar D, Variance C",
	`3_Dminpl_Cexp_Cvar` = "Min planar D, Expected C, Variance C"
)

g = NULL

for (n in c(10,11,12,13,14,15,16,17,18,19,20,25,50,100,200,500)) {
	data = read.data(n)
	
	data <- data %>%
		mutate(ir_type = factor(ir_type, levels = names(LABELS), labels = LABELS))
	
	times = data %>%
		ggplot(aes(x = N, y = time, colour = ir_type)) +
		geom_point(aes(shape = ir_type)) +
		geom_line(aes(linetype = ir_type)) +
		labs(
			title = paste0("n= ", n),
			x = "Number of trees",
			y = "Average execution time (ms)",
			linetype = "Feature",
			shape = "Feature",
			colour = "Feature"
		) +
		scale_x_log10() +
		scale_y_log10()
	
	occupancy = data %>%
		ggplot(aes(x = N, y = unique, colour = ir_type)) +
		geom_point(aes(shape = ir_type)) +
		geom_line(aes(linetype = ir_type)) +
		labs(
			title = paste0("n= ", n),
			x = "Number of trees",
			y = "Number of unique trees",
			linetype = "Feature",
			shape = "Feature",
			colour = "Feature"
		) +
		scale_x_log10() +
		scale_y_log10()
	
	data$speedup = 0
	
	data[data$ir_type == LABELS["1_Dminpl"], ]$speedup =
		data[data$ir_type == LABELS["0"], ]$time/data[data$ir_type == LABELS["1_Dminpl"], ]$time
	data[data$ir_type == LABELS["1_Cexp"], ]$speedup =
		data[data$ir_type == LABELS["0"], ]$time/data[data$ir_type == LABELS["1_Cexp"], ]$time
	data[data$ir_type == LABELS["2_Dminpl_Cvar"], ]$speedup =
		data[data$ir_type == LABELS["0"], ]$time/data[data$ir_type == LABELS["2_Dminpl_Cvar"], ]$time
	data[data$ir_type == LABELS["2_Dminpl_Cexp"], ]$speedup =
		data[data$ir_type == LABELS["0"], ]$time/data[data$ir_type == LABELS["2_Dminpl_Cexp"], ]$time
	data[data$ir_type == LABELS["3_Dminpl_Cexp_Cvar"], ]$speedup =
		data[data$ir_type == LABELS["0"], ]$time/data[data$ir_type == LABELS["3_Dminpl_Cexp_Cvar"], ]$time
	
	data[data$ir_type == LABELS["0"], ]$speedup = 1
	
	speedup = data %>%
		ggplot(aes(x = N, y = speedup, colour = ir_type)) +
		geom_point(aes(shape = ir_type)) +
		geom_line(aes(linetype = ir_type)) +
		labs(
			title = paste0("n= ", n),
			x = "Number of trees",
			y = "Speed up factor",
			linetype = "Feature",
			shape = "Feature",
			colour = "Feature"
		) +
		scale_x_log10() +
		scale_y_log10()
	
	if (is.null(g)) {
		g = occupancy | times | speedup + plot_layout(guides = 'collect')
	}
	else {
		g = g / (occupancy | times | speedup + plot_layout(guides = 'collect'))
	}
}

pdf("full.pdf", height = 50, width = 15)
plot(g + plot_layout(guides = 'collect'))
dev.off()
