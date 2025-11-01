#include <windows.h>
#include <stdio.h>


/* PURPOSE: Helper for asynchronous database operations.
    IT WILL BE CONVERTED INTO A .H FILE 

*/

// TODO CREATE A CHILD PROCESS
// 

void exec_operation(const char* operation){

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si,sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi,sizeof(pi));

    char buffer[256];
    snprintf(buffer,sizeof(buffer),"cmd.exe /c \"%s\" ",operation);

   if (!CreateProcess(
            NULL,           // lpApplicationName
            buffer,            // lpCommandLine (must be writable)
            NULL,           // lpProcessAttributes
            NULL,           // lpThreadAttributes
            FALSE,          // bInheritHandles
            0,              // dwCreationFlags
            NULL,           // lpEnvironment
            NULL,           // lpCurrentDirectory
            &si,            // lpStartupInfo
            &pi             // lpProcessInformation
    )) {
        printf("CreateProcess failed (%d)\n", GetLastError());
        return;
    }
    // wait for the process to finish
    //WaitForSingleObject(pi.hProcess,INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);



}
#define arrSize(arr) ( sizeof((arr)) / sizeof((arr)[0]) )
int main(int argc, char* argv[]){
    /* if (argc < 2){
        printf("USAGE : <OPERATION> \n");
        return 1;
    } */
   /*  const char* ops[] = {
        "dir",
        "mkdir js",
        "dir",
        "rmdir js"
    }; */

   // int size = arrSize(ops);
    exec_operation("python -m http.server 8000");
    printf("Python HTTP server started on http://localhost:8000\n");
    return 0;
}

