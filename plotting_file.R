library(ggplot2) # data visualization 
library(viridis) # data visualization 
library(ggh4x) # data visualization 
library(dplyr) # data manipulation
library(tidyverse)
library(cowplot)
library(gridExtra)
library(ggplotify)
library(patchwork)
library(ggpattern)

## themes and colors
options(scipen=999)
facet_nested_theme <- theme(
  strip.background = element_rect(fill = "white", colour = "grey", linetype="dotted", linewidth=0.2), 
  panel.background = element_rect(fill='white', color='grey'),
  panel.border = element_blank(),
  panel.grid.major = element_blank(), 
  panel.grid.minor = element_blank(),
  legend.key = element_rect(color = "transparent"),
  panel.spacing.x = unit(0,"line"),
  panel.spacing.y = unit(0,"line"),
  axis.text.x = element_text(angle = 90, vjust = 0.2, hjust=0.2))


combo_df <- read_csv("combo_data_file.csv")
combo_df <- combo_df %>% mutate(cross_prob= as.factor(cross_prob), pop_size = as.factor(pop_size), mut_prob = as.factor(mut_prob), size_punishment = as.factor(size_punishment))
combo_df <- combo_df %>% mutate(world_type_name = case_when(world_type == 0 ~ "Coevolutionary", world_type == 1 ~ "Dual control", world_type == 2 ~ "Single control", .default = "ERROR"))

unique(combo_df$test)
## cross prob
combo_df %>% filter(generation %% 25 == 0) %>% filter(test=="pop_size")%>%
  ggplot(aes(x = generation, y = min_error_bits_mismatching, color = pop_size, fill = pop_size)) + 
  stat_summary(fun.data="mean_cl_boot", geom=c("smooth")) + 
  facet_nested(~world_type_name) + facet_nested_theme + 
  labs(x = "Generation", y = "Best solution number\nof mismatching bits", color = "Population\nsize") +
  scale_color_viridis_d(option="mako") + scale_fill_viridis_d(option="mako")+
  guides(fill = "none")

combo_df %>% filter(generation %% 25 == 0) %>%filter(test=="pop_size")%>%
  ggplot(aes(x = generation, y = min_error_length, color = pop_size, fill = pop_size)) + 
  stat_summary(fun.data="mean_cl_boot", geom=c("smooth")) + 
  facet_nested(~world_type_name) + facet_nested_theme+ 
  labs(x = "Generation", y = "Best solution length", color = "Population\nsize") +
  scale_color_viridis_d(option="mako") + scale_fill_viridis_d(option="mako") +
  guides(fill = "none")

combo_df %>% filter(generation %% 25 == 0) %>% filter(test=="pop_size")%>%
  ggplot(aes(x = generation, y = min_A_error_length/min_error_length, color = pop_size, fill = pop_size)) + 
  stat_summary(fun.data="mean_cl_boot", geom=c("smooth")) + 
  facet_nested(~world_type_name) + facet_nested_theme+ 
  labs(x = "Generation", y = "Proportion of best solution\ncomprised of program A", color = "Population\nsize") +
  scale_color_viridis_d(option="mako") + scale_fill_viridis_d(option="mako") +
  guides(fill = "none")

## cross prob
combo_df %>% filter(generation %% 25 == 0) %>% filter(test=="cross_prob")%>%
  ggplot(aes(x = generation, y = min_error_bits_mismatching, color = cross_prob, fill = cross_prob)) + 
  stat_summary(fun.data="mean_cl_boot", geom=c("smooth")) + 
  facet_nested(~world_type_name) + facet_nested_theme + 
  labs(x = "Generation", y = "Best solution number\nof mismatching bits", color = "Crossover\nprobability") +
  scale_color_viridis_d(option="plasma") + scale_fill_viridis_d(option="plasma")+
  guides(fill = "none")

combo_df %>% filter(generation %% 25 == 0) %>%filter(test=="cross_prob")%>%
  ggplot(aes(x = generation, y = min_error_length, color = cross_prob, fill = cross_prob)) + 
  stat_summary(fun.data="mean_cl_boot", geom=c("smooth")) + 
  facet_nested(~world_type_name) + facet_nested_theme+ 
  labs(x = "Generation", y = "Best solution length", color = "Crossover\nprobability") +
  scale_color_viridis_d(option="plasma") + scale_fill_viridis_d(option="plasma") +
  guides(fill = "none")

combo_df %>% filter(generation %% 25 == 0) %>% filter(test=="cross_prob")%>%
  ggplot(aes(x = generation, y = min_A_error_length/min_error_length, color = cross_prob, fill = cross_prob)) + 
  stat_summary(fun.data="mean_cl_boot", geom=c("smooth")) + 
  facet_nested(~world_type_name) + facet_nested_theme+ 
  labs(x = "Generation", y = "Proportion of best solution\ncomprised of program A", color = "Crossover\nprobability") +
  scale_color_viridis_d(option="plasma") + scale_fill_viridis_d(option="plasma") +
  guides(fill = "none")

## mut prob
combo_df %>% filter(generation %% 25 == 0) %>% filter(test=="mut_prob")%>%
  ggplot(aes(x = generation, y = min_error_bits_mismatching, color = mut_prob, fill = mut_prob)) + 
  stat_summary(fun.data="mean_cl_boot", geom=c("smooth")) + 
  facet_nested(~world_type_name) + facet_nested_theme + 
  labs(x = "Generation", y = "Best solution number\nof mismatching bits", color = "Mutation\nprobability") +
  scale_color_viridis_d(option="viridis") + scale_fill_viridis_d(option="viridis")+
  guides(fill = "none")

combo_df %>% filter(generation %% 25 == 0) %>% filter(test=="mut_prob")%>%
  ggplot(aes(x = generation, y = min_error_length, color = mut_prob, fill = mut_prob)) + 
  stat_summary(fun.data="mean_cl_boot", geom=c("smooth")) + 
  facet_nested(~world_type_name) + facet_nested_theme+ 
  labs(x = "Generation", y = "Best solution length", color = "Mutation\nprobability") +
  scale_color_viridis_d(option="viridis") + scale_fill_viridis_d(option="viridis") +
  guides(fill = "none")

combo_df %>% filter(generation %% 25 == 0) %>% filter(test=="mut_prob")%>%
  ggplot(aes(x = generation, y = min_A_error_length/min_error_length, color = mut_prob, fill = mut_prob)) + 
  stat_summary(fun.data="mean_cl_boot", geom=c("smooth")) + 
  facet_nested(~world_type_name) + facet_nested_theme+ 
  labs(x = "Generation", y = "Proportion of best solution\ncomprised of program A", color = "Mutation\nprobability") +
  scale_color_viridis_d(option="viridis") + scale_fill_viridis_d(option="viridis") +
  guides(fill = "none")


## punsh
combo_df %>% filter(generation %% 25 == 0) %>% filter(test=="punish_factor")%>%
  ggplot(aes(x = generation, y = min_error_bits_mismatching, color = size_punishment, fill = size_punishment)) + 
  stat_summary(fun.data="mean_cl_boot", geom=c("smooth")) + 
  facet_nested(~world_type_name) + facet_nested_theme + 
  labs(x = "Generation", y = "Best solution number\nof mismatching bits", color = "Length\npunishment") +
  scale_color_viridis_d(option="turbo") + scale_fill_viridis_d(option="turbo")+
  guides(fill = "none")

combo_df %>% filter(generation %% 25 == 0) %>% filter(test=="punish_factor")%>%
  ggplot(aes(x = generation, y = min_error_length, color = size_punishment, fill = size_punishment)) + 
  stat_summary(fun.data="mean_cl_boot", geom=c("smooth")) + 
  facet_nested(~world_type_name) + facet_nested_theme+ 
  labs(x = "Generation", y = "Best solution length", color = "Length\npunishment") +
  scale_color_viridis_d(option="turbo") + scale_fill_viridis_d(option="turbo") +
  guides(fill = "none")

combo_df %>% filter(generation %% 25 == 0) %>% filter(test=="punish_factor")%>%
  ggplot(aes(x = generation, y = min_A_error_length/min_error_length, color = size_punishment, fill = size_punishment)) + 
  stat_summary(fun.data="mean_cl_boot", geom=c("smooth")) + 
  facet_nested(~world_type_name) + facet_nested_theme+ 
  labs(x = "Generation", y = "Proportion of best solution\ncomprised of program A", color = "Length\npunishment") +
  scale_color_viridis_d(option="turbo") + scale_fill_viridis_d(option="turbo") +
  guides(fill = "none")


nrow(combo_df  %>% filter(generation == max(generation)))
combo_df  %>% filter(generation == max(generation)) %>% 
  group_by(world_type_name) %>%
  summarize(found = sum(min_error_bits_mismatching == 0)) %>%  
  ggplot(aes(x= world_type_name, y = found, fill = world_type_name)) + geom_col() +
  facet_nested_theme + labs(x = "World type", y = "Count of correct solutions") + guides(fill = "none") + 
  theme(axis.text.x = element_text(angle = 90, vjust = 0.2, hjust=0.2))
