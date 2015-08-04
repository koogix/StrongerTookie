PHP的多任务类，基于PHP的PCNTL及POSIX扩展，实现并发运行多任务功能！

应用场景如，群发推送消息，批量数据处理等！

使用说明：

（1）包含 classes/MooMPC.class.php 文件
（2）使用 MooMPC::getInstance() 初始化
（3）调用 init() 的话是检测是否启用了 pcntl 及 posix 扩展，如果确定本地环境已经启用，可以忽略
（4）调用 task() 设置子任务执行函数
（5）运行 exec() 开始执行创建并执行多个子任务

实现例程：
    
    具体参看 examples/main.php 文件！
    主要部分如下，可以多次调用task()设置多种不同的任务
    ----------------------------------
    MooMPC::getInstance()
        ->init()
        ->task('procFunc1')
        ->task('procFunc2', 2, 'a', $idx)
        ->task(array($pro, 'func1'), 1, 'ok')
        ->task(array($pro, 'func2'), 1, 'hello', 'world')
        ->exec();
    ------------------------------------
