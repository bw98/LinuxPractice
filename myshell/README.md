## **myshell的实现**
#### 一、要求
###### 1、实现单个命令，如: ls
###### 2、命令可以带参数，如：ls -l /tmp
###### 3、可以带有一个输入或者输出的重定向。
##### 4、可以带一个管道符命令
##### 5、可以后台运行
###### 6、输入exit或logout退出。

#### 二、错误处理
###### 1、输入错误的命令格式报错
###### 2、输入不存在的命令报错

#### 三、关键函数
###### 1、打印myshell前标（print_prompt）
###### 2、获取用户的输入（get_input）
###### 3、参数解析函数（explain_input）
###### 4、执行命令函数（do_cmd）
###### 5、命令查找函数（find_command）
