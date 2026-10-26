int main(void)
{
    const char *url = "https://example.com//path";
    const char *text = "characters /* not a comment */ here";
    char slash = '/';
    char star = '*';
    char quote = '\'';
    // this comment should disappear
    return url[0] + text[0] + slash + star + quote;
}
