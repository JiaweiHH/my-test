# 一个简单的测试程序
* make my-test编译
* 测试进程的nice = 1， static_prio = 121

## 进程实时内存占用可视化

### RUN

1. `dmesg --follow > mydmesg.log` 生成可视化配置文件
2. 在浏览器打开 [task-rss.html](./realtime-task-rss-html/task-rss.html)

### 参考

[echarts SanKey Diagram example](https://echarts.apache.org/examples/zh/editor.html?c=sankey-levels)
[SanKey config option](https://echarts.apache.org/zh/option.html#series-sankey)