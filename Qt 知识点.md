# Qt 知识点

```c++
// 获取桌面
QDesktopWidget* deskTop = QApplication::desktop();

QRect rect1 =  deskTop->availableGeometry();

// 获取默认屏幕分辨率
QRect rect2 =  deskTop->screenGeometry();

QRect rect3 =  deskTop->frameGeometry();

// qGuiApp: QGuiApplication instance
qDebug() << qGuiApp->primaryScreen()->availableGeometry();
```





## QPaltte

QPalette类包含每个小部件状态的颜色组。

调色板由三种颜色组组成:活动、禁用和非活动。Qt中的所有小部件都包含一个调色板，并使用它们的调色板来绘制自己。这使得用户界面易于配置和保持一致。

如果您创建一个新的小部件，我们强烈建议您使用调色板中的颜色，而不是硬编码特定的颜色。

