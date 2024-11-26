#include <stdlib.h>
#include <fcgi_stdio.h>

int main()
{
    while (FCGI_Accept() >= 0)
    {
        printf("Content-Type:text\r\n\r\n");
        printf("clintip= %s\r\n", getenv("REMOTE_ADDR"));
        printf("request= %s\r\n", getenv("REQUEST_METHOD"));
        printf("query_string= %s\r\n", getenv("QUERY_STRING"));

    }
    return 0;
}