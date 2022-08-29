#include <ti/screen.h>
#include <keypadc.h>
#include <ti/getcsc.h>
#include <stdio.h>
#include <ti/ui.h>

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

const uint24_t colors[4] = {
    0xD8EE, 0x9251, 0x01B2, 0x76BD};

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

void bumpUpSelection(int *response, int question)
{
    if (*response < 3)
    {
        selectionDisplay(response, *response + 2, question);
    }
}

void bumpDownSelection(int *response, int question)
{
    if (*response > 0)
    {
        selectionDisplay(response, *response, question);
    }
}

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

int main(void)
{

    float cisCount = 0, biCount = 0, gayCount = 0;

    float cisbiCount = 0, cisgayCount = 0, bigayCount = 0;

    os_ClrHome();

    os_PutStrFull("This is an \"Am I gay?\" test port. Use arrow keys or number keys to choose your answer, then press enter to continue, press clear to quit, or press mode to skip to the results. There are no accuracy guarantees. All rights are reserved. Press any key to begin.");

    while (!os_GetCSC())
    {
        // wait
    };

    os_FontSelect(os_LargeFont);

    for (int i = 0; i < 11; i++)
    {
        int response = 0;

        selectionDisplay(&response, 1, i);

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
            break;
        }
    afterSwitchStatement:;
    }
afterForLoop:
    os_ClrHome();
    char *resultString = "";

    float finalCisCount = calculateFinalValuePartial(cisCount, cisbiCount, biCount, cisgayCount, gayCount);
    float finalBiCount = calculateFinalValuePartial(biCount, cisbiCount, cisCount, bigayCount, gayCount);
    float finalGayCount = calculateFinalValuePartial(gayCount, bigayCount, biCount, cisgayCount, cisCount);

    float answeredSum = finalCisCount + finalBiCount + finalGayCount;

    sprintf(resultString, "You are %.1f%% straight, %.1f%% bisexual, and %.1f%% gay.", finalCisCount / answeredSum * 100, finalBiCount / answeredSum * 100, finalGayCount / answeredSum * 100);

    os_PutStrFull(resultString);

    while (!os_GetCSC())
    {
        // wait
    };

    return 0;
}