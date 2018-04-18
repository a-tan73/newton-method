/*******************************************************/
/*    Convienient*/
/*******************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/errno.h>
#include <sys/types.h>

#ifdef _MAIN
#define _Global
#else
#define _Global	extern
#endif


#ifdef TRUE
#undef TRUE
#endif
#ifdef FALSE
#undef FALSE
#endif

#define TRUE    1
#define FALSE   0

/*-----------------------------------------------------*/
/* Valuable type aliases                               */
/*-----------------------------------------------------*/
#ifndef uchar
#define uchar unsigned char
#endif
#ifndef uint
#define uint unsigned int
#endif
#ifndef ushort
#define ushort unsigned short
#endif
#ifndef ulong
#define ulong unsigned long
#endif

typedef struct {
    int      Num;
    int     *LenArry;
    char   **StrArry;
    char    *Str;
} StrSepSt;

typedef struct {
    _Bool   OverWriteMode;
    _Bool   VerboseMode;
    _Bool   SilentMode;
    FILE   *ErrorStream;
    int     Argc;
    char  **Argv;
    int     ArgcCopy;
    char  **ArgvCopy;
    char   *ProgNmae;
    char   *DefaultOption;
    char   *AddOption;
} MyToolStruct;

//#ifdef _MAIN
MyToolStruct MyToolInfo;
//#else
//extern MyToolStruct MyToolInfo;
//#endif

int GeneralErrExit(int ErrCode, char *Message, ...)
{
    va_list	 argptr;
    char    *str;
    
    str = strerror(ErrCode);
    
    fprintf(MyToolInfo.ErrorStream, "%s",MyToolInfo.Argv[0]);
    fprintf(MyToolInfo.ErrorStream, ": ");
    
    va_start(argptr, Message);
        vfprintf(MyToolInfo.ErrorStream, Message, argptr);
    va_end(argptr);
    
    fprintf(MyToolInfo.ErrorStream, "%s", str);
    fputc('\n', MyToolInfo.ErrorStream);
    
    exit(ErrCode);    
    return(ErrCode);
}

_Global int (*ErrExit)(int ErrCode, char *Message, ...) = GeneralErrExit;

int UserAbortExit(char *Message, ...)
{
    va_list	 argptr;
    
    fprintf(MyToolInfo.ErrorStream, "%s", MyToolInfo.Argv[0]);
    fprintf(MyToolInfo.ErrorStream, ": ");
    
    va_start(argptr, Message);
        vfprintf(MyToolInfo.ErrorStream, Message, argptr);
    va_end(argptr);
    
    exit(-1);    
    return(-1);
}

_Global int (*UserAbort)(char *Message, ...) = UserAbortExit;
_Global int (*Abort)(char *Message, ...) = UserAbortExit;

_Bool ArgDel(int DelPos, int DelNum)
{
    int     i, j, k;
    
    if(DelPos < 1 || DelPos > MyToolInfo.ArgcCopy - 1) return FALSE;
    if(MyToolInfo.ArgcCopy <= DelPos + DelNum - 1) return FALSE;
    
    k = MyToolInfo.ArgcCopy;
    j = DelPos + DelNum;
    
    for(i = 0; i < k - j; i++){
        MyToolInfo.ArgvCopy[DelPos + i] = MyToolInfo.ArgvCopy[i + j];
    }
    
    MyToolInfo.ArgcCopy -= DelNum;
    return TRUE;
}

_Bool CheckSimpleOption(char *Option)
{
    int     i, Pos;
    int     count = 0;
    
    for(i = 1; i < MyToolInfo.ArgcCopy; i++){
        if(strcmp(MyToolInfo.ArgvCopy[i], Option) == 0){
            Pos = i;
            count ++;
        }
    }
    if(count == 0) return FALSE;
    if(count > 1 ) {
        Abort("Duplicated Option %s was found\n", Option);
    }
    ArgDel(Pos, 1);
    return TRUE;
}    

void MyToolInit(int argc, char **argv)
{
    int     i;
    
    MyToolInfo.OverWriteMode = FALSE;
    MyToolInfo.VerboseMode   = FALSE;
    MyToolInfo.SilentMode    = FALSE;
    MyToolInfo.ErrorStream   = stderr;

    
    
    errno = 0;
    MyToolInfo.Argv      = argv;
    MyToolInfo.ArgvCopy = (char **)malloc(sizeof(char *) * (argc+1));
    if(MyToolInfo.Argv == NULL) goto ERR_EXIT;
        
    MyToolInfo.Argc     = argc;
    MyToolInfo.ArgcCopy = argc;

    for(i = 0; i < argc; i++){
        errno = 0;
        MyToolInfo.ArgvCopy[i]  = strdup(argv[i]);
        if(MyToolInfo.Argv[i] == NULL) goto ERR_EXIT;
        // printf("%s\n",MyToolInfo.Argv[i]);
    }
    
    
    
    return;
    
ERR_EXIT:
    fprintf(MyToolInfo.ErrorStream, 
                "MyToolInfo initilize was faild.\n");
    perror(NULL);
    exit(errno);

}

        
_Bool YorN( void )
{
    int     c;
    
    c = getchar();
    c = toupper(c);
    if(c == 'Y')
            return( TRUE );
    else
            return( FALSE );
}

_Bool CheckFileExistence( char *FileName )
{
    FILE    *fp;
    
    if( (fp = fopen( FileName, "r")) == NULL )
        return(FALSE);
    else{
        fclose(fp);
        return(TRUE);
    }
}





void *Malloc(size_t size)
{
    char *p;
    
    errno = 0;
    p = (char *)malloc(size);
    if(p == NULL) ErrExit(errno, "Malloc was faild\n");
    
    return (void *)p;
}

char *Strdup(const char *str)
{
    char    *p;
    
    errno = 0;
    p = strdup(str);
    if(p == NULL) ErrExit(errno, "Strdup was faild\n");
            
    return p;
}

FILE *ReadOpenBin(char *FileName)
{
    FILE *fp;
    
    errno = 0;
    fp = fopen(FileName, "rb");
    if(NULL == fp){
        ErrExit(errno, "Read File Open Error [%s]\n", FileName);
    }
    clearerr(fp);
    return(fp);
}

FILE *ReadOpenTxt(char *FileName)
{
    FILE *fp;
    
    errno = 0;
    fp = fopen(FileName, "r");
    if(NULL == fp){
        ErrExit(errno, "Read File Open Error [%s]\n", FileName);
    }
    clearerr(fp);
    return(fp);
}

FILE *AppendOpenTxt(char *FileName)
{
    FILE *fp;
    
    errno = 0;
    fp = fopen(FileName, "a");
    if(NULL == fp){
        ErrExit(errno, "Read File Open Error [%s]\n", FileName);
    }
    clearerr(fp);
    return(fp);
}


FILE *WriteOpenBin(char *FileName)
{
    FILE *fp;
    
    errno = 0;
    fp = fopen(FileName, "wb");
    if(NULL == fp){
        ErrExit(errno, "Write File Open Error [%s]\n", FileName);
    }
    clearerr(fp);
    return(fp);
}

FILE *WriteOpenTxt(char *FileName)
{
    FILE *fp;
    
    errno = 0;
    fp = fopen(FileName, "w");
    if(NULL == fp){
        ErrExit(errno, "Write File Open Error [%s]\n", FileName);
    }
    clearerr(fp);
    return(fp);
}

FILE *CreateOpenBin(char *FileName)
{
    FILE *fp;
    
    if(MyToolInfo.OverWriteMode == FALSE){
        if(CheckFileExistence(FileName)){
            fprintf(stderr, 
                "[%s] already exists. Over write? (y/n)   ", FileName);
            if(YorN() == TRUE) remove(FileName);
            else UserAbort("User aborted ...\n");
        }
    }
    
    
    errno = 0;
    fp = fopen(FileName, "wb");
    if(NULL == fp){
        ErrExit(errno, "File Creation Error [%s]\n", FileName);
    }
    clearerr(fp);
    return(fp);
}
FILE *CreateOpenTxt(char *FileName)
{
    FILE *fp;
    
    if(MyToolInfo.OverWriteMode == FALSE){
        if(CheckFileExistence(FileName)){
            fprintf(stderr, 
                "[%s] already exists. Over write? (y/n)   ", FileName);
            if(YorN() == TRUE) remove(FileName);
            else UserAbort("User aborted ...\n");
        }
    }
    
    
    errno = 0;
    fp = fopen(FileName, "w");
    if(NULL == fp){
        ErrExit(errno, "File Creation Error [%s]\n", FileName);
    }
    clearerr(fp);
    return(fp);
}

size_t Fwrite(void *ptr, size_t size, size_t num, FILE *fp)
{
    size_t WriteElem;
    
    errno = 0;
    WriteElem = fwrite(ptr, size, num, fp);
    if(ferror(fp)) ErrExit(errno, "File write error\n");
    
    return WriteElem;
}

size_t Fread(void *ptr, size_t size, size_t num, FILE *fp)
{
    size_t WriteElem;
    
    errno = 0;
    WriteElem = fread(ptr, size, num, fp);
    if(ferror(fp)) ErrExit(errno, "File read error\n");
    
    return WriteElem;
}

char *Fgets(char *buff, int size, FILE *fp)
{
    errno = 0;
    fgets(buff, size, fp);
    if(ferror(fp)) ErrExit(errno, "fgets faild\n");
}

int Fseek(FILE *fp, long Offs, int Whence)
{
    int     i;
    
    errno = 0;
    i = fseek(fp, Offs, Whence);
    if(i == -1) ErrExit(errno, "Fseek error\n");
    
    return i;
}

long Ftell(FILE *fp)
{
    long    l;
    
    errno = 0;
    l = ftell(fp);
    if(l == -1L) ErrExit(errno, "Ftell error\n");
    
    return l;
}

long GetFileSize(char *FileName)
{
    FILE    *fp;
    long     l;
    
    fp = ReadOpenBin(FileName);
    Fseek(fp, 0L, SEEK_SET);
    Fseek(fp, 0L, SEEK_END);
    
    l = Ftell(fp);
    fclose(fp);
    
    return l;
}


char *StrUpper(char *str)
{
    int i, leng;
    
    leng = strlen(str);
    if(leng == 0) return str;
    
    for(i = 0; i < leng; i++){
        str[i] = toupper(str[i]);
    }
    
    return str;
}

char *CutRddSpace(char *Str, int mode)
{
    int      len, i;
    char    *Dst, *ptr, *dptr;
    _Bool    flg, Tflg;
    
    if(mode == '\t')
            Tflg = FALSE;
    else
            Tflg = TRUE;
    
    len = strlen(Str);
    if(len == 0){
        ErrExit(0, "CutRddSpace() need at least 1 string length\n");
    }
    
    flg = FALSE;
    for(i = 0; i < len; i++){
        if(Str[i] == ' ') continue;
        if(Tflg == TRUE && Str[i] == '\t') continue;
        ptr = Str + i;
        flg = TRUE;
        break;
    }
    
    if(flg == FALSE) return NULL;
    Dst = (char *)Malloc((size_t)len);
    
    len = strlen(ptr);
    dptr = Dst;
    flg = FALSE;
    for(i = 0; i < len; i++){
        if(ptr[i] == ' '){
            flg = TRUE;
            continue;
        }
        if(Tflg == TRUE && ptr[i] == '\t'){
            flg = TRUE;
            continue;
        }
            
        if(flg == TRUE){
            *dptr++ = ' ';
            flg = 0;
        }
        *dptr++ = ptr[i];
    }
    return Dst;
}

int StrTokenSep(StrSepSt *Stsp, char *SrcStr, int Delimit)
{
    int      i, numd, Slen, count;
    char    *ScutStr, *ptr, *spos;
    _Bool    TabFlg = FALSE;
    
    /* Cut Redundant space */
    ScutStr = CutRddSpace(SrcStr, Delimit);
    
    printf("[%s]\n", ScutStr);
    
    /* Count Delimiter number */
    Slen = strlen(ScutStr);
    for(i = 0, numd = 0; i < Slen; i++){
        if(ScutStr[i] == Delimit) numd++;
    }
    
    if(numd == 0){
        free(ScutStr);
        return 0;
    }
    
  
    printf("numd = %d\n", numd);
    
    numd++;
    
    Stsp->Num     = numd;
    Stsp->LenArry = (int *)Malloc((size_t)(sizeof(int) * numd));
    Stsp->StrArry = (char **)Malloc((size_t)(sizeof(char *) * numd));
    Stsp->Str     = ScutStr;
    
    count = 0;
    ptr  = ScutStr;
    spos = ScutStr;
    numd = 0;
    for(i = 0 ; i < Slen; i++){
        if( *ptr == Delimit ){
            Stsp->LenArry[numd] = count; count = 0;
            Stsp->StrArry[numd] = spos;  spos = ptr + 1;
            numd++;
            *ptr++ = '\0';
            continue;
        }
        count++;
        ptr++;
    }
    
    Stsp->LenArry[Stsp->Num - 1] = count;
    Stsp->StrArry[Stsp->Num - 1] = spos; 
    
    
    for(i = 0; i < Stsp->Num; i++){
        printf("LenArry[%d] = %d   [%s]\n", 
                i,  Stsp->LenArry[i], Stsp->StrArry[i]);
    }
    
    
    return Stsp->Num;
}


void RelStrTokenSep(StrSepSt *Stsp)
{
    int      i, num;
    
    if(Stsp->Num == 0) return;
    
    free(Stsp->Str);
    free(Stsp->LenArry);
    free(Stsp->StrArry);
}
