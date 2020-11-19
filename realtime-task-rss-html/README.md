# 进程地址分布可视化

## RUN


1. 使用[工具](../show-task-physical-usage/README.md) 生成可视化配置文件
2. 在浏览器打开 [address-usage.html](./address-usage.html)
3. （如有必要按需调节 canvas 画布大小和节点迭代次数 layoutIterations）
4. 选择想要可视化的配置文件，等待时间与节点迭代次数正相关

## 参考

[echarts SanKey Diagram example](https://echarts.apache.org/examples/zh/editor.html?c=sankey-levels)
[SanKey config option](https://echarts.apache.org/zh/option.html#series-sankey)