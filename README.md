# 一个简单的测试程序
* make my-test编译
* 测试进程的nice = 1， static_prio = 121

## 进程实时内存占用可视化

### RUN

1. `dmesg --follow > mydmesg.log` 记录内存使用信息
2. `sudo trace-cmd record -e 'sched_*'` 记录调度信息
3. `time ./my_test` 运行测试程序，并记录运行时间，结束后手动停止上两个程序
4. 在浏览器打开 [task-rss.html](./realtime-task-rss-html/task-rss.html)，选择要可视化的 log 数据

### 参考

[echarts SanKey Diagram example](https://echarts.apache.org/examples/zh/editor.html?c=sankey-levels)
[SanKey config option](https://echarts.apache.org/zh/option.html#series-sankey)