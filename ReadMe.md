



## GASDebugger 使用说明

![image-20250730090056559](https://raw.githubusercontent.com/VJien/img/master/imgimage-20250730090056559.png)

如上图，GAS自带的DebugUI很不好用

![image-20250730090631212](https://raw.githubusercontent.com/VJien/img/master/imgimage-20250730090631212.png)

以上是GASDebugger界面



![image-20250730090750687](https://raw.githubusercontent.com/VJien/img/master/imgimage-20250730090750687.png)

在windows页面下的底部会有启动按钮

游戏运行以后，插件会尝试自动识别场景内拥有技能组件的玩家，如没有识别到或者玩家类是延迟创建的，那需要手动选择一下（可以选择所有对象包括怪物）

![image-20250730090958728](https://raw.githubusercontent.com/VJien/img/master/imgimage-20250730090958728.png)

选中以后即可实时监听GAS数据





### 游戏设置

![image-20250730113353230](https://raw.githubusercontent.com/VJien/img/master/imgimage-20250730113353230.png)

在ProjectSettings->Game中有GASDebugger的游戏设置

- Debugger Widget： 需要手动设置为 EU_GASDebuggerMain，这个是实际启动的蓝图EditorWidget
- Enable Logging: 如果开启，那么会开启本地日志文件的保存功能
- AttributeLogUpdateFrequency： 属性更新频率，如果是0那么每次更新都会记录到日志
- AttributeLogDecimalPrecision： 属性保留小数位数





### 日志功能

![image-20250730113824558](https://raw.githubusercontent.com/VJien/img/master/imgimage-20250730113824558.png)



通过上图按钮打开日志目录

每次一次在关闭PIE的时候会生成一个日志文件夹

![image-20250730113920632](https://raw.githubusercontent.com/VJien/img/master/imgimage-20250730113920632.png)

![image-20250730113932477](https://raw.githubusercontent.com/VJien/img/master/imgimage-20250730113932477.png)

All中保存了所有其他4种类型的日志

![image-20250730114009959](https://raw.githubusercontent.com/VJien/img/master/imgimage-20250730114009959.png)

























