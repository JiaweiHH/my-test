# 一个简单的测试程序
1. make my-test编译
2. IO测试, n个进程分别读取372MB的数据再写入到一个新的文件中去. direct I/O和buffer I/O
3. cpu+memory测试, n个进程中按照一定比例分配cpu和cpu+mem型进程
4. 所有进程运行结束时间作为result对比

## 进程实时内存占用可视化

更改优先级保证只打印测试进程的数据。测试进程的nice = 1， static_prio = 121
### 获取log文件

1. `dmesg --follow > mydmesg.log` 生成可视化配置文件
2. 在浏览器打开 [task-rss.html](./realtime-task-rss-html/task-rss.html)

## 参考

[echarts SanKey Diagram example](https://echarts.apache.org/examples/zh/editor.html?c=sankey-levels)
[SanKey config option](https://echarts.apache.org/zh/option.html#series-sankey)
[几种不同的I/O实现](https://www.cnkirito.moe/linux-io-benchmark/)
[buffer I/O和direct I/O](https://blog.csdn.net/jyxmust/article/details/88354485)