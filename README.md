# 2048 Temporal Difference Learning with C++ and SFML
This repository contains the implementation of three Temporal Difference Learning algorithms applied to the game 2048 using C++ and the SFML library. This project builds upon the previously developed [2048 game in C++ and SFML](https://github.com/Triadziuch/2048).

![2024-06-2019-48-40-00 03 45 630-00 04 07 256-ezgif com-optimize](https://github.com/Triadziuch/2048-AI-Temporal-Difference/assets/75269577/8b92d981-0c34-4ee8-835f-95a3ee57411b)

# Overview
The purpose of this project is to create an intelligent agent capable of playing and winning the game 2048 at 1-ply without using human expertise or performing game tree search. Three Temporal Difference Learning algorithms were implemented:

* Q-Learning
* TD-State
* TD-Afterstate

These algorithms were trained and evaluated based on the study by Marcin Szubert and Wojciech Jaśkowski.


# Project Goal
The primary goal of this project is to understand and expand knowledge of artificial intelligence algorithms, specifically Temporal Difference Learning algorithms. By applying these algorithms to the game 2048, I aim to explore their effectiveness and efficiency in a practical, real-world scenario. The project provides insights into how AI can autonomously learn to make decisions and improve performance over time through self-play and reinforcement learning techniques.


# Algorithms

## Q-Learning
Q-Learning is a model-free reinforcement learning algorithm that seeks to learn the value of the optimal policy directly. It updates the value of the action taken based on the reward received and the estimated value of the next state.

## TD-State
TD-State focuses on learning the value of states directly rather than actions. It evaluates the expected future rewards for each state and updates the state values accordingly.

## TD-Afterstate
TD-Afterstate is similar to TD-State but evaluates the "afterstate" resulting from an action rather than the state itself. This method can lead to more accurate value estimations in games with deterministic transitions and is first described in Marcin Szubert and Wojciech Jaśkowski study.


# Algorithms Performance
The table below shows the performance of the different learning algorithms:

| Learning Rate | Q-Learning (%)      | TD-State (%)        | TD-Afterstate (%)    | Q-Learning Score     | TD-State Score       | TD-Afterstate Score  |
| ------------- | ------------------- | ------------------- | -------------------- | -------------------- | -------------------- | -------------------- |
| 0.0010        | 0.0001 ± 0.0000     | 76.92 ± 1.30        | 78.74 ± 1.65         | 4854.81 ± 46.33      | 37303.64 ± 640.38    | 38362.97 ± 764.90    |
| 0.0025        | 30.64 ± 1.69        | 79.90 ± 0.94        | 83.43 ± 1.13         | 16519.17 ± 519.38    | 38679.55 ± 478.50    | 39220.15 ± 514.10    |
| 0.0050        | 26.73 ± 1.76        | 79.64 ± 0.83        | 80.69 ± 0.82         | 14939.18 ± 542.01    | 36187.95 ± 381.47    | 35740.57 ± 357.10    |
| 0.0075        | 36.90 ± 1.25        | 75.85 ± 0.73        | 74.12 ± 0.76         | 18226.61 ± 367.39    | 31867.43 ± 335.45    | 30657.45 ± 367.08    |
| 0.0100        | 28.99 ± 1.15        | 70.55 ± 0.69        | 68.37 ± 0.69         | 15304.22 ± 350.55    | 29063.30 ± 276.18    | 27557.62 ± 278.03    |


# Acknowledgements
This project is based on the [research by Marcin Szubert and Wojciech Jaśkowski](https://www.cs.put.poznan.pl/wjaskowski/pub/papers/Szubert2014_2048.pdf), presented at the CIG 2014 IEEE Conference on Computational Intelligence and Games.


# Dependencies
Before running this application, ensure that you have installed the Visual C++ Redistributable for Visual Studio 2015-2022. You can download it from the official Microsoft website [here](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads).


# License
Copyright © 2024 by Triadziuch

This project is licensed under the MIT License - see the LICENSE file for details.


# Third-party Components
This project utilizes the SFML library, which is licensed under the zlib/png license.
