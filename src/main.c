#include <ti/screen.h>
#include <keypadc.h>
#include <ti/getcsc.h>
#include <stdio.h>
#include <ti/ui.h>

/*
cis, bi, and gay are to be used if an answer is soley answerable by someone who is that
eg "I am gay."
cisgay, cisbi, and bigay can be used if it could be answerable by either
eg "I am not gay."
nil should be used if it could be answered by any or none
eg "I like oranges."
*/
enum sexuality
{
    cis,
    bi,
    gay,
    cisgay,
    cisbi,
    bigay,
    nil
};

const char *questions[11][5] = {
    {"1. Why did you decide to take this test?", "(%d) To check how gay I am.", "(%d) All my friends kept saying I was gay and should take this test.", "(%d) Just to have fun!", "(%d) To make sure I'm straight!"},
    {"2. Have you ever looked at a person of the same sex and felt attracted in a sexual way?", "(%d) Yes, all the time!", "(%d) Sometimes, but I also have opposite sex crushes!", "(%d) Never, I am not gay at all!", "(%d) Very rarely, but sometimes!"},
    {"3. If your best friend confessed to you that they are gay, you would:", "(%d) Welcome them to the club!", "(%d) Start flirting with them!", "(%d) Make a joke about it.", "(%d) Tell them that sometimes you feel the same way."},
    {"4. Have you ever worn or fantasized about wearing clothes of another sex?", "(%d) All the time!", "(%d) Sometimes, but I wouldn't go out in public like that.", "(%d) Never!", "(%d) Rarely, but sometimes I may wear my partner's clothes!"},
    {"5. If there were no people left in the world except for someone of the same sex, you would:", "(%d) Be happy, as no one would ever flirt with my partner!", "(%d) Be dissapointed you are losing out on the other sex.", "(%d) Fall into despair as you will never have a sexual partner again!", "(%d) Be confused what just happened and do none of the others."},
    {"6. Have you ever kissed someone of the same sex?", "(%d) Yes, I do it all the time!", "(%d) Once or twice!", "(%d) No way I'm doing that!", "(%d) I don't pull."},
    {"7. How frequently do you dream about the same sex romantically?", "(%d) All of my dreams are about the same sex!", "(%d) Most of the time.", "(%d) Rarely to never.", "(%d) I don't dream about people."},
    {"8. Would you be comfortable with a gay colleague flirting with you at a (possibly hypothetical) workplace?", "(%d) Sure, it's a lot of fun!", "(%d) I guess so, it depends on the situation!", "(%d) I would feel very awkward about it!", "(%d) It's never appropriate to flirt at work!"},
    {"9. Your friends decided to go party at a gay bar, you would:", "(%d) Feel like you're going home! Let's go party!", "(%d) Get excited to check out a new venue!", "(%d) Feel threatened or uncomfortable.", "(%d) Get secretly excited, as you always wanted to try being super fruity."},
    {"10. If your partner offered you a three way \"sleepover\" with someone of the same sex, you would:", "(%d) Feel excited about it!", "(%d) Never! I only like the opposite sex!", "(%d) Do it as I don't care about the sex of another person!", "(%d) Again, I don't do much pulling."},
    {"11. What instrument do you play?", "(%d) You play some exotic instrument called the clarinet.", "(%d) You play many instruments, but shun all clarinets.", "(%d) You play any other good instrument.", "(%d) You don't play an instrument."}};

const char *overwriteString = "                              ";

const enum sexuality keys[11][4] = {
    {gay, bigay, nil, cisgay},
    {bigay, bi, cis, bi},
    {gay, bigay, nil, gay},
    {gay, bigay, cis, bi},
    {gay, bi, cis, nil},
    {bigay, bi, cis, nil},
    {gay, bi, cisbi, nil},
    {bigay, bigay, cisbi, nil},
    {gay, bigay, cis, gay},
    {gay, cis, bi, nil},
    {bigay, cis, nil, nil}};

// this displays the required information given the question number, the answer number, and a pointer to the response number variable
void selectionDisplay(int *response, int number, int question)
{
    os_HomeUp();

    os_PutStrFull(questions[question][0]);
    os_PutStrLine(overwriteString);
    os_NewLine();
    {
        char *tempString = "";
        sprintf(tempString, questions[question][number], number);
        os_PutStrFull(tempString);
    }

    // this section gets rid of any extra text remaining
    unsigned int row = 0;
    unsigned int col = 0;
    os_GetCursorPos(&row, &col);
    for (int i = row; i < 9; i++)
    {
        os_PutStrLine(overwriteString);
        os_NewLine();
    }
    os_PutStrLine(overwriteString);

    *response = number - 1;
}

// for navigating selection with arrow keys
void bumpUpSelection(int *response, int question)
{
    if (*response < 3)
    {
        selectionDisplay(response, *response + 2, question);
    }
}

// for navigating selection with arrow keys
void bumpDownSelection(int *response, int question)
{
    if (*response > 0)
    {
        selectionDisplay(response, *response, question);
    }
}

// no longer used, but is a somewhat useful solution for calculating final values. guaranteed to return a result
float calculateFinalValuePartial(float value, float partialA, float valueA, float partialB, float valueB)
{
    float fullPartialA, fullPartialB;

    if (value + valueA != 0)
    {
        fullPartialA = partialA * value / (value + valueA);
    }
    else
    {
        fullPartialA = partialA * 0.5;
    }

    if (value + valueB != 0)
    {
        fullPartialB = partialB * value / (value + valueB);
    }
    else
    {
        fullPartialB = partialB * 0.5;
    }

    return value + fullPartialA + fullPartialB;
}

/*
a much more advanced method to calculate proportions
p_a, p_b, and p_c are pointers to the final probability outputs
c_a, c_b, and c_c are the straight answer counts
c_ab, c_ac, c_bc are the mixed answer counts
c_abc is the full answer count
*/
void calculateFinalPortions(
    float *p_a, float *p_b, float *p_c,
    float c_a, float c_b, float c_c,
    float c_ab, float c_ac, float c_bc, float c_abc)
{
    *p_a = 0.33;
    *p_b = 0.33;
    *p_c = 0.34;

    float c = c_a + c_b + c_c + c_ab + c_ac + c_bc + c_abc;

    // dont ask, it just works
    for (int i = 0; i < 20; i++)
    {
        float t_p_a = (c_a +
                       c_ab * (*p_a) / (*p_a + *p_b) +
                       c_ac * (*p_a) / (*p_a + *p_c) +
                       c_abc * (*p_a)) /
                      c;
        float t_p_b = (c_b +
                       c_ab * (*p_b) / (*p_a + *p_b) +
                       c_bc * (*p_b) / (*p_b + *p_c) +
                       c_abc * (*p_b)) /
                      c;
        float t_p_c = (c_c +
                       c_ac * (*p_c) / (*p_a + *p_c) +
                       c_bc * (*p_c) / (*p_b + *p_c) +
                       c_abc * (*p_c)) /
                      c;
        *p_a = t_p_a;
        *p_b = t_p_b;
        *p_c = t_p_c;
    }
}

int main(void)
{

    // initializes variable for tracking sums
    float cisCount = 0, biCount = 0, gayCount = 0;

    float cisbiCount = 0, cisgayCount = 0, bigayCount = 0;

    float cisbigayCount = 0;

    os_ClrHome();

    os_PutStrFull("This is an \"Am I gay?\" test port. Use arrow keys or number keys to choose your answer, then press enter to continue, press clear to quit, or press mode to skip to the results. There are no accuracy guarantees. All rights are reserved. Press any key to begin.");

    while (!os_GetCSC())
    {
        // wait
    };

    for (int i = 0; i < 11; i++)
    {
        int response = 0;

        selectionDisplay(&response, 1, i);

        // loops till it gets an appropriate response
        while (true)
        {
            uint8_t key = os_GetCSC();

            switch (key)
            {
            case sk_1:
                selectionDisplay(&response, 1, i);
                continue;
            case sk_2:
                selectionDisplay(&response, 2, i);
                continue;
            case sk_3:
                selectionDisplay(&response, 3, i);
                continue;
            case sk_4:
                selectionDisplay(&response, 4, i);
                continue;
            case sk_Up:
                bumpDownSelection(&response, i);
                continue;
            case sk_Down:
                bumpUpSelection(&response, i);
                continue;
            case sk_Left:
                bumpDownSelection(&response, i);
                continue;
            case sk_Right:
                bumpUpSelection(&response, i);
                continue;
            case sk_Enter:
                break;
            case sk_Clear:
                return 1;
            case sk_Mode:
                goto afterForLoop;
                break;
            case sk_Del:
                goto afterSwitchStatement;
                break;
            default:
                continue;
            }

            break;
        }
        // adds to appropriate count
        switch (keys[i][response])
        {
        case cis:
            cisCount++;
            break;
        case bi:
            biCount++;
            break;
        case gay:
            gayCount++;
            break;
        case cisbi:
            cisbiCount++;
            break;
        case cisgay:
            cisgayCount++;
            break;
        case bigay:
            bigayCount++;
            break;
        case nil:
            cisbigayCount++;
            break;
        }
    afterSwitchStatement:;
    }
afterForLoop:;
    char *resultString = "";

    // calculates results
    float finalCisPortion;
    float finalBiPortion;
    float finalGayPortion;

    calculateFinalPortions(
        &finalCisPortion, &finalBiPortion, &finalGayPortion,
        cisCount, biCount, gayCount,
        cisbiCount, cisgayCount, bigayCount, cisbigayCount);

    // generates results string
    sprintf(resultString, "You are %.1f%% straight, %.1f%% bisexual, and %.1f%% gay. Thank you for taking my \"Am I gay?\" test! Share it with all your friends! Take it again! Use it to define your personality! I don't care.",
            finalCisPortion * 100, finalBiPortion * 100, finalGayPortion * 100);

    // prints out results string
    os_ClrHome();

    os_PutStrFull(resultString);

    // fills in blanks
    {
        unsigned int row = 0;
        unsigned int col = 0;
        os_GetCursorPos(&row, &col);
        for (int i = row; i < 9; i++)
        {
            os_PutStrLine(overwriteString);
            os_NewLine();
        }
        os_PutStrLine(overwriteString);
    }

    while (!os_GetCSC())
    {
        // wait
    }

    return 0;
}