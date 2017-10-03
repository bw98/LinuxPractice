/*************************************************************************
	> File Name: myshell.c
	> Author: bw98
	> Mail: 786016746@qq.com
	> Created Time: 2017年10月02日 星期一 10时00分54秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<string.h>
#include<dirent.h>
#include<sys/stat.h>
#define normal 0 //普通命令
#define out_redirect 1 //输出重定向
#define in_redircet 2 //输入重定向
#define have_pipe 3 //命令中有管道

void print_prompt(); //打印myshell提示符
void get_input(char *); //获取一条用户输入的命令，并存在buf中
void explain_input(char *,char a[][256], int *); //解析buf中的命令，把命令与每个选项放在arglist中
void do_cmd(int , char a[][256]); //执行arglist中的命令，argcount是执行命令的参数个数
int find_cmd(char *); //在当前目录、/bin、/usr/bin下查找命令的可执行程序

int main()
{
    int argcount = 0;
    int i;
    char *buf = NULL;
    char arglist[100][256];
    buf = (char *)malloc(256);
    if(buf == NULL)
    {
        perror("malloc failed");
        exit(-1);
    }   
    while(1)
    {
        /*刷新buf*/
        memset(buf,0,256);
        print_prompt();
        get_input(buf);
        if((strcmp(buf,"exit\n") == 0) || (strcmp(buf,"logout\n") == 0))
            break;
        /*arglist 字符串数组初始化*/
        for(i = 0; i < 100; i++ )    
            arglist[i][0] = '\0';
        argcount = 0;
        explain_input(buf, arglist, &argcount);
        do_cmd(argcount,arglist);
    }
    
    if(buf != NULL)
    {
        free(buf);
        buf = NULL;
    }
    exit(0);  //正常退出
}

/*打印myshell提示符*/
void print_prompt()
{
    printf("bw98@Bw98:~/myshell$ ");
}

/*获得一条用户输入的命令，将参数包括中间的空格整体存入一个一位数组buf当中，等待下一个函数进行参数解析，并判断如果参数长度若大于256则终止程序，输入的命令以‘\n’为结束标志。*/
void get_input(char *buf)
{
    int len = 0;
    char ch;
    ch = getchar();
    while(len != 256 && ch != '\n' )
    {
      buf[len++] = ch;
      ch = getchar();
    }
    if(len == 256)
    {
        printf("输出过长，退出程序\n");
        exit(-1);
    }
    //改字符为字符串
    buf[len] = '\n';
    len++;
    buf[len] = '\0';
}

/*解析buf中的命令，将结果存入arglist中，命令以回车符号\n结束。*/
/*如输入命令为“ls -l /tmp”，则arglist[0] arglist[1] arglist[2]分别为ls -l 和 /tmp */
void explain_input(char *buf, char arglist[100][256], int * argcount)
{
    char *p = buf;
    char *q = buf;
    int number = 0;
    while(1)
    {
        if(p[0] == '\n')
            break;
        if(p[0] == ' ')
            p++;
        else{
            q = p;
            number = 0;
            while((q[0]!=' ') && (q[0]!='\n' )){
                number++;
                q++;
            }
            strncpy(arglist[*argcount], p, number+1);
            arglist[*argcount][number] = '\0';
            *argcount = *argcount + 1;
            p = q;
        }
    }
}

/*执行arglist中存放的命令，argcount为待执行命令的参数个数*/
void do_cmd(int argcount, char arglist[100][256])
{
    int i;    
    int flag = 0; /*标志是否有多个 > < | 符号(本程序无法处理多符号情况),或者格式不对(如：< cat) */ 
    int how = 0; /*标志是否有有 > < | 符号 */
    char *arg[argcount+1];
    char *argnext[argcount+1];
    int background = 0; /*后台运行符的标识符 */
    char *file;
    pid_t pid;
    int fd;
    int status; /*存放子进程的状态信息并传递给wait */

    /*将命令取出*/
    for(i = 0; i < argcount; i++){
        arg[i] =(char *)arglist[i];
    }
    arg[argcount] = NULL; //arg[argcount]必须为NULL，因为下面要用到的exec函数族需要有argv参数，它的argv参数用来提供给main函数的argv参数，而argv参数是一个以NULL结尾的字符串数组
    
    /*查看命令行是否有后台运行符& */
    for(i = 0; i < argcount ; i++){
        if(strncmp(arg[i],"&",1) == 0){
            if(i == argcount-1){
                background = 1;
                arg[argcount-1] = NULL;
                break;
            }
            else{
                printf("wrong command!\n");
                return;
            }
        }
    }
    /* 查看是否含有 > < | 符号 */
    for(i = 0; arg[i] != NULL; i++){
        if(strcmp(arg[i], ">") == 0){
            flag++;
            how = out_redirect;
            if(arg[i+1] == NULL)
                flag++;
        }
        if(strcmp(arg[i], "<") == 0){
            flag++;
            how = in_redircet;
            if(i == 0)
                flag++;
        }
        if(strcmp(arg[i], "|") == 0){
            flag++;
            how = have_pipe;
            if(arg[i+1] == NULL)
                flag++;
            if(i == 0)
                flag++;
         }
    }
    
    /*命令格式出错或输入多个 > < | 符号 */
    if(flag > 1){
        printf("wrong command!\n");
        return;
    }

    /*处理单符号情况 */
    if(how == out_redirect){
        for(i = 0; arg[i] != NULL; i++){
            if(strcmp(arg[i],">") == 0){
                file = arg[i+1]; //文件在arg[i+1]处
                arg[i] = NULL;
            }
        }    
    }
    if(how == in_redircet){
        for(i = 0; arg[i] != NULL; i++){
            if(strcmp(arg[i],"<") == 0){
                file = arg[i+1];
                arg[i] = NULL;
            }
        }
    }
    if(how == have_pipe){  //把管道后边的部分存入argnext中，因为它是一个可执行的Shell命令
        for(i = 0; arg[i] != NULL; i++){
            if(strcmp(arg[i],"|") == 0){
                arg[i] = NULL;
                int j;
                for(j = i+1; arg[j] != NULL; j++){
                    argnext[j-i-1] = arg[j];
                }
                argnext[j-i-1] = arg[j];
                break;
            }
        }
    }

    /*重定向与管道命令的实现*/
    if((pid = fork()) < 0){
        printf("fork error!\n");
        return;
    }   
    
    switch(how){
        case 0:
            /*输入命令中不含有>、<、| 这些符号 */
            /*子进程执行输入的命令*/
            if(pid == 0){
                if(!find_cmd(arg[0])){
                    printf("%s: command is not found\n",arg[0]);
                    exit(0);
                }
                execvp(arg[0], arg);
                exit(0);
            }
            break;
        case 1:
            /*输入命令中含有输出重定向符> */
            if(pid == 0){
                if(!find_cmd(arg[0])){
                    printf("%s: command is not found\n",arg[0]);
                    exit(0);
                }
                fd = open(file,O_RDWR|O_CREAT|O_TRUNC,0644);
                dup2(fd,1);
                execvp(arg[0], arg);
                exit(0);
            }
            break;
        case 2:
            /*输入命令中含有输入重定向符< */
            if(pid == 0){
                if(!find_cmd(arg[0])){
                        printf("%s: command is not found\n", arg[0]);
                        exit(0);
                }
                fd = open(file, O_RDONLY, 0644);
                dup2(fd,0);
                execvp(arg[0], arg);
                exit(0);
            }
            break;
        case 3:
            /*输入命令中含有管道符| */
            if(pid == 0){
                pid_t pid2;
                int status2;
                int fd2;
                
                if((pid2 = fork()) < 0){
                    printf("fork2 error\n");
                    return;
                }
                else if(pid2 == 0){
                    if(!find_cmd(arg[0])){
                        printf("command is not found!\n");
                        exit(0);
                    }
                    //子进程将左边的命令的stdout写入fd2所指向的文件表项
                    fd2 = open("/tmp/youdonotknowfile", O_WRONLY|O_CREAT|O_TRUNC, 0644);
                    dup2(fd2,1);      /*子进程执行写入的好处是父进程的stdin不受影响 */
                    execvp(arg[0],arg); 
                    exit(0);
                }
                
                /*父进程挂起，子进程写入左边的命令后父进程继续执行 */
                if(waitpid(pid2, &status2, 0) == -1){
                    printf("wait for child process error\n");
                }

                //查看文件路径是否存在
                if(!find_cmd(argnext[0])){
                    printf("%s: command is not found\n",argnext[0]);
                    exit(0);
                }
                
                /*父进程执行管道右侧的shell命令，同时把其stdin改为fd2所指向的文件表项 */
                fd2 = open("/tmp/youdonotknowfile", O_RDONLY);
                dup2(fd2,0);
                execvp(argnext[0],argnext);
                
                /*删除暂存的文件 */
                if(remove("/tmp/youdonotknowfile"))
                    printf("remove error\n");
                exit(0);

            }
            break;
        default:
            break;
    }

    /*如果有&，表示后台执行，父进程直接返回而不等待子进程 */
    if(background == 1){
        printf("background process id is %d\n",pid);
        return;
    }

    /*父进程等待子进程结束 */
    if(waitpid(pid, &status, 0) == -1){
        printf("wait for child process error\n");
    }
        
}    


int find_cmd(char *command)
{
    DIR *dp; /*DIR结构体指针，在opendir()时产生，指向目录，可用于判断目录是否存在 */
    struct dirent *dirp;  //dirent结构体指针，指向目录下的所有文件，用于判断目录内容是否存在
    char *path[] = {"./","/bin","/usr/bin",NULL};

    /*使当前目录下的程序可以执行*/
    if(strncmp(command, "./", 2) == 0)
        command = command +2; //取出不含"./"的子串
    
    /*分别在当前目录、/bin和/usr/bin目录查找要执行的程序*/
    int i =0;
    while(path[i] != NULL){
        if((dp = opendir(path[i])) == NULL){
            printf("can not open /bin !\n");
        }
        while((dirp = readdir(dp)) != NULL){
            if(strcmp(dirp->d_name,command) == 0){
                closedir(dp);
                return 1; //查到命令，返回1
            }
        }
        closedir(dp);
        i++;
    }
    return 0;
}
