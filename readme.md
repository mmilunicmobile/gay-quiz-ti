# It's a gay quiz. 
What did you expect?

But realistically, you answer the questions. This could be close the most mathematically advanced "Am I gay?" test there is, and is most definitely the most advanced "Am I gay?" test, if not the only "Am I gay?" test for the TI-84 Plus CE.

## Toolkit

This was compiled from C using the [CE C/C++ Toolchain](https://github.com/CE-Programming/toolchain). 

## Algorithm
For the algorithm, for simplification, someone can have same-sex feelings, or opposite-sex feelings. This means each question can give away the info that they have same-sex feelings, do not have same-sex feelings, have opposite-sex feelings, and/or do not have opposite-sex feelings.

|sexuality|same-sex|opposite-sex|
|-|-|-|
|gay|true|false|
|bisexual|true|true|
|straight|false|true|

I do not have asexuality programmed in on this test and do not really plan to do so as I'm allready kinda tired of the project.

This table shows how the different response weights are treated by the program.

|weight|gay|bi|cis|
|-|-|-|-|
|nil|maybe|maybe|maybe|
|gay|true|false|false|
|bi|false|true|false|
|cis|false|false|true|
|cisbi|false|maybe|maybe|
|cisgay|maybe|false|maybe|
|bigay|maybe|maybe|false|

For calculating the final percentages, I have two rules:
 - If a person answers a question, the way they responded to that question in the final percents is some combination of all the possibilites of its maybes, e.g. a cisgay weight comes out to be some portion cis and some portion gay.
 - That portion is the same for each question and is determined by the final portions that are returned at the end of the test.

From this we can create recursive equations to solve for the final gay portions, bisexual portions, and straight portions.

$P_a$ is the final straight portion, $P_b$ is the final bisexual portion, and $P_c$ is the final gay portion. $C_a$ is the number of cis weighted responses, $C_b$ is the number of bi weighted responses, and $C_c$ is the number of gay weighted responses. Variables like $C_{ac}$, $C_{ab}$, $C_{bc}$, and $C_{abc}$ are combined weights and are the number of cisgay, cisbi, bigay, and nil weighted responses respectively. $C$ is the total number of responses.

Below is a system of equations that could theoretically be solved to give exact answers for the three portions system.

$$\left(C_a + \frac{C_{ab}*P_a}{P_a + P_b} + \frac{C_{ac}*P_a}{P_a + P_c} + C_{abc} * P_a\right)=P_a*C$$
$$\left(C_b + \frac{C_{ab}*P_b}{P_a + P_b} + \frac{C_{bc}*P_b}{P_b + P_c} + C_{abc} * P_b\right)=P_b*C$$
$$\left(C_c + \frac{C_{ac}*P_c}{P_a + P_c} + \frac{C_{ac}*P_c}{P_a + P_c} + C_{abc} * P_c\right)=P_c*C$$
$$P_a + P_b + P_c = 1$$
I have not solved it.

Theoretically it should be solvable as there are only three unknowns, $P_a$, $P_b$, and $P_c$, and there are at least 3 equations. 
I am not sure the fourth equation is neccesary to solve the system, but I am not an expert at this sort of thing either. 
What we can do is plug in guess unknowns on the left sides of the first three equations, and then calculate what that would make the unknowns on the right sides. We can then use these new unknowns as our new guess unknowns and repeat. This will converge to a set of portions which solve the system and is the system I use in this program.

## Graphics
Most of the graphics are done simply using the toolchain's built in str display functions. I do avoid clearing the screen, as that causes flickering, and instead prefer writing over the previous screen and adding whitespace to fill every line. This creates a much less flickery experience.

## Questions
Most questions were taken from [this quiz](https://www.arealme.com/gay-test/en/), however some answers and questions have been modified. The weights on the answers were also all chosen by me.