# STK3700_Responsetime

In this exercise, a reaction time measurement is performed with the STK3700 card. According to the instructions on the card display, the user has to press the push buttons on the card. During the measurement, the luminance off the room is measured, and from these 2 data (reaction time + luminance), information can be obtained on how much the reaction time is affected by the fact that it is dark or bright. The results will be stored on Linux and we will analyse the data here. Reaction time measurements can also be initiated from the Linux computer. During the reaction time measurement, a countdown will occur on the LCD display, with the numbers (3,2,1) flashing, after which the Gecko symbol will flash on the LCD display within 5 seconds, at which point the user should press BTN_1. 3 reaction times are measured and the results are averaged. This averaging will be done on the microcontroller and the averaged data will be sent to Linux. The results are permanently stored on Linux. On Linux PC it will be possible to plot the reaction times as a function of luminance.
