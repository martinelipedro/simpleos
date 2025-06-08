#include <stdio.h>

int main()
{
    int n, cont;

    printf("Type a number: ");
    scanf("%d", &n);

    for (int i = 0; i < n; ++i)
    {
        printf("Type a number: ");
        scanf("%d", &n);

        if (n > 0)
            ++cont;
        else
            break;
    }

    printf("%d positives were typed.\n", cont);

    return 0;
}