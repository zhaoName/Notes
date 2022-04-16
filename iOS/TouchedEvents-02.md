# UIResponder、UIGestureRecognizer、UIControl


本文主要介绍 UIResponder、UIGestureRecognizer、UIControl 对于触摸事件的响应优先级。


<br>

## 一、UIGestureRecognizer


手势分为离散型手势（discrete gestures）和持续型手势（continuous gesture）。系统提供的离散型手势包括点按手势（`UITapGestureRecognizer`）和轻扫手势（`UISwipeGestureRecognizer`），其余均为持续型手势。

两者主要区别在于状态变化过程：

离散型：

- 识别成功：Possible —> Recognized

- 识别失败：Possible —> Failed

持续型：

- 识别成功：Possible —> Began —> [Changed] —> Ended

- 识别失败：Possible —> Began —> [Changed] —> Cancel


### 0x01 离散型手势

定义一个 `YellowView` 视图，重写 `UIResponder` 的响应事件方法。

```
@implementation YellowView

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    NSLog(@"%s", __func__);
}

- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    NSLog(@"%s", __func__);
}

- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    NSLog(@"%s", __func__);
}

@end
```

然后将 `YellowView` 添加到控制器的视图上，并为它绑定一个 `UITapGestureRecognizer` 手势


```Objective-C
@implementation GestureViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    UITapGestureRecognizer *tapGes = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(didTouchYellowView)];
    [self.yellowView addGestureRecognizer:tapGes];
}

- (void)didTouchYellowView
{
    NSLog(@"%s", __func__);
}

@end
```

![](../Images/iOS/TouchesEvents/ResponsePriority_01.png)

点击 `YellowView ` 得到如下打印结果：

```Objective-C
-[YellowView touchesBegan:withEvent:]
-[GestureViewController didTouchYellowView]
-[YellowView touchesCancelled:withEvent:]
```

从日志上看出 `YellowView` 最后 `Cancel` 了对触摸事件的响应，而正常应当是触摸结束后，`YellowView` 的 `touchesEnded:withEvent:` 的方法被调用才对。另外，期间还执行了手势识别器绑定的事件 。从官方文档找到了这样的解释：

> A window delivers touch events to a gesture recognizer before it delivers them to the hit-tested view attached to the gesture recognizer. Generally, if a gesture recognizer analyzes the stream of touches in a multi-touch sequence and doesn’t recognize its gesture, the view receives the full complement of touches. If a gesture recognizer recognizes its gesture, the remaining touches for the view are cancelled. The usual sequence of actions in gesture recognition follows a path determined by default values of the cancelsTouchesInView, delaysTouchesBegan, delaysTouchesEnded properties:

大致理解是，window 在将事件传递给 hit-tested view之 前，会先将事件传递给相关的手势识别器并由手势识别器优先识别。若手势识别器成功识别了事件，就会取消 hit-tested view 对事件的响应；若手势识别器没能识别事件，hit-tested view 才完全接手事件的响应权。

**一句话概括：`UIGestureRecognizer` 比 `UIResponder` 具有更高的事件响应优先级！！**


```Objective-C
// UIGestureRecognizerSubclass.h

// mirror of the touch-delivery methods on UIResponder
// UIGestureRecognizers aren't in the responder chain, but observe touches hit-tested to their view and their view's subviews
// UIGestureRecognizers receive touches before the view to which the touch was hit-tested
- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event;
- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event;
- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event;
- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event;
- (void)touchesEstimatedPropertiesUpdated:(NSSet<UITouch *> *)touches API_AVAILABLE(ios(9.1));
```





```Objective-C

```


```Objective-C

```


```Objective-C

```

<br>

## 二、

<br>

## 三、

<br>



<br>

**Reference**

- [UIGestureRecognizer](https://developer.apple.com/documentation/uikit/uigesturerecognizer?language=objc)

- [iOS触摸事件全家桶](https://www.jianshu.com/p/c294d1bd963d)

<br>