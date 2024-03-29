# 触摸事件

本文主要介绍触摸事件的传递和响应。

<br>

## 一、`UITouch`、`UIEvent`、`UIResponder`

### 0x01 `UITouch`

`UITouch` 对象表示屏幕上 touch 的位置、大小、移动和压力。

通过传递给 `responder` 的 `UIEvent` 来获取 `UITouch` 对象，`UITouch` 提供以下信息：

- touch 发生的 view 或 window。

- touch 位于 view 或 window 的位置。
- touch 大致半径。
- touch 压力大小（支持 3D Touch 或 Apple Pencil 的设备）。

整数类型的 `tapCount` 属性表示点击屏幕的次数，`UITouch.Phase` 属性表示处于 `began`、`moved`、`ended`、`cancelled`等阶段。

```Objective-C
typedef NS_ENUM(NSInteger, UITouchPhase) {
    UITouchPhaseBegan,             // whenever a finger touches the surface.
    UITouchPhaseMoved,             // whenever a finger moves on the surface.
    UITouchPhaseStationary,        // whenever a finger is touching the surface but hasn't moved since the previous event.
    UITouchPhaseEnded,             // whenever a finger leaves the surface.
    UITouchPhaseCancelled,         // whenever a touch doesn't end but we need to stop tracking (e.g. putting device to face)
    UITouchPhaseRegionEntered   API_AVAILABLE(ios(13.4), tvos(13.4)) API_UNAVAILABLE(watchos),  // whenever a touch is entering the region of a user interface
    UITouchPhaseRegionMoved     API_AVAILABLE(ios(13.4), tvos(13.4)) API_UNAVAILABLE(watchos),  // when a touch is inside the region of a user interface, but hasn’t yet made contact or left the region
    UITouchPhaseRegionExited    API_AVAILABLE(ios(13.4), tvos(13.4)) API_UNAVAILABLE(watchos),  // when a touch is exiting the region of a user interface
};
```

下面两个属性来区分 `UITouch` 所属对象。

```Objective-C
// 事件所属 window
@property(nullable,nonatomic,readonly,strong) UIWindow *window;
// 事件所属 view
@property(nullable,nonatomic,readonly,strong) UIView *view;
```

### 0x02 `UIEvent`

`UIEvent` 描述用户与 app 的单次交互。

应用可以接收不同类型的事件（event），包括触摸事件（touch events）、运动事件（motion events）、远程控制事件（remote-control events）和按下物理按键事件（press events）。触摸事件是最常见的事件，发送给触摸的视图。运动事件由 UIKit 触发，并与 Core Motion framework 运动事件进行区分。远程控制事件允许 responder 对象接收外部配件的指令（如耳机），以便管理音视频播放。Press events 代表与 game controller、AppleTV remote 或其他有物理按键设备的交互。可以使用 `type` 和 `subtype` 属性判断事件类型。

事件类型如下：

```Objective-C
typedef NS_ENUM(NSInteger, UIEventType) {
    UIEventTypeTouches,
    UIEventTypeMotion,
    UIEventTypeRemoteControl,
    UIEventTypePresses API_AVAILABLE(ios(9.0)),
    UIEventTypeScroll      API_AVAILABLE(ios(13.4), tvos(13.4)) API_UNAVAILABLE(watchos) = 10,
    UIEventTypeHover       API_AVAILABLE(ios(13.4), tvos(13.4)) API_UNAVAILABLE(watchos) = 11,
    UIEventTypeTransform   API_AVAILABLE(ios(13.4), tvos(13.4)) API_UNAVAILABLE(watchos) = 14,
};
```

Touch event 对象包含与事件相关的 touch。Touch event 对象包含一个或多个 touch，每个 touch 都由 `UITouch` 对象表示。当发生触摸事件时，系统将事件路由至合适的响应者，并调用 ` touchesBegan:withEvent:` 方法，responder 提取 touch 中的数据，并作出适当响应。

在多点触摸序列中，向 app 传递更新的触摸数据时，`UIKit` 会复用 `UIEvent` 对象。因此，不要持有 `UIEvent` 对象及其中的数据。如果需要在响应者方法之外使用 `UIEvent`、`UITouch` 数据，应在响应者方法中处理数据，并复制到自定义的数据结构中。

### 0x03 `UIResponder`

每个响应者都是一个`UIResponder` 对象，即所有派生自 `UIResponder` 的对象，本身都具备响应事件的能力。因此以下类的实例都是响应者：

- `UIView`

- `UIViewController`
- `UIApplication`
- `AppDelegate`

想要处理特定类型事件，responder 必须重写对应方法，如下：

```Objective-C
//手指触碰屏幕，触摸开始
- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(nullable UIEvent *)event;
//手指在屏幕上移动
- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(nullable UIEvent *)event;
//手指离开屏幕，触摸结束
- (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(nullable UIEvent *)event;
//触摸结束前，某个系统事件中断了触摸，例如电话呼入
- (void)touchesCancelled:(NSSet<UITouch *> *)touches withEvent:(nullable UIEvent *)event;
```


<br>

## 二、事件的生命周期

当指尖触碰屏幕的那一刻，一个触摸事件就在系统中生成了。经过 IPC 进程间通信，事件最终被传递到了合适的应用中。然后在应用中经过传递、响应到最终释放。大致过程如下：

![](../Images/iOS/TouchesEvents/TouchesEvents_01.png)

**系统响应阶段**

- 手指触碰屏幕，屏幕感应到触碰后，将事件交由 `IOKit.framework` 处理

- 通过 `IOKit.framework` 将触摸事件包装成 `IOHIDEvent` 对象
- 然后系统通过 mach port (IPC 进程间通信) 将 `IOHIDEvent` 对象转发给 SpringBoard.app
- SpringBoard.app 是 iOS 系统桌面 App，它只接收按键、触摸、加速、接近传感器等几种 Event。SpringBoard.app 会找到可以响应这个事件的 App，并通过 mach port(IPC 进程间通信) 将 `IOHIDEvent` 对象转发给这个 App。

**APP响应阶段**

- APP 进程的 mach port 接受到 SpringBoard 进程传递来的触摸事件，主线程的 runloop 被唤醒，触发了 `source1` 回调。

- `source1` 回调内部触发了 `source0` 回调，`source0` 内部回调将接收到的 `IOHIDEvent` 对象封装成 `UIEvent` 对象。
- `Soucre0` 回调内部调用 `UIApplication` 的 `+[sendEvent:]` 方法，将 `UIEvent` 传给 `UIWindow`。至此 APP 将正式开始对于触摸事件的响应。
- `source0` 回调内部将触摸事件添加到 UIApplication 对象的事件队列中。事件出队后，UIApplication开始一个寻找最佳响应者的过程，这个过程又称`hit-testing`，细节将后面详述。
- 寻找到最佳响应者后，接下来的事情便是事件在响应链中的被响应了，关于响应链相关的内容详见。
- 事实上，事件除了被响应者消耗，还能被手势识别器或是 `target-action` 模式捕捉并消耗掉。其中涉及对触摸事件的响应优先级，详见。
- 触摸事件历经坎坷后要么被某个响应对象捕获后释放，要么致死也没能找到能够响应的对象，最终释放。至此触摸事件的使命就算终结了。runloop 若没有其他事件需要处理，也将重归于眠，等待新的事件到来后唤醒。

<br>

## 三、事件传递

每个事件的理想宿命是被能够响应它的对象响应后释放，然而响应者诸多，事件一次只有一个，谁都想把事件抢到自己碗里来，为避免纷争，就得有一个先后顺序，也就是得有一个响应者的优先级。因此这就存在一个寻找事件最佳响应者（又称第一响应者 first responder）的过程，目的是找到一个具备最高优先级响应权的响应对象（the most appropriate responder object），这个过程叫做 Hit-Testing，那个命中的最佳响应者称为 hit-tested view。

### 0x01 谁能成为第一响应者

`UIKit` 根据事件的类型将对象指定为事件的第一响应者。事件类型包括：

| Event type | First responder |
| --- | --- |
| Touch events | The view in which the touch occurred. |
| Press events| The object that has focus. |
| Shake-motion events | The object that you (or UIKit) designate. |
| Remote-control events | The object that you (or UIKit) designate. |
| Editing menu messages | The object that you (or UIKit) designate. |

### 0x02 事件自下而上传递

应用接收到事件后先将其置入事件队列中以等待处理。出队后，application 首先将事件传递给当前应用最后显示的窗口（`UIWindow`）询问其能否响应事件。若窗口能响应事件，则传递给子视图询问是否能响应，子视图若能响应则重复询问子视图的子视图。子视图询问的顺序是优先询问后添加的子视图，因为后添加会默认显示在上层，成为第一响应者的几率更大。事件传递顺序如下：

```
UIApplication(下) -> UIWindow -> subview -> ... -> subview (上)
```

创建如下视图：

![](../Images/iOS/TouchesEvents/TouchesEvents_02.png)

当我们点击 `View B.1` 的时候，查找第一响应者的过程大致如下(深度优先搜索)：

![](../Images/iOS/TouchesEvents/TouchesEvents_03.png)

- `UIWindow` 将事件传递给 `MainVIew`

- `MainVIew` 判断自身能否响应事件，若能则倒序遍历 `MainVIew` 的子视图，判断触摸点的位置在 `View B` 范围内
- `View B` 判断自身能否响应事件，若能再倒序遍历 `View B` 的子视图，判断触摸点的位置在 `View B.1` 范围内
- 查找到 `View B.1` 是第一响应者。

查找第一响应者要判断视图是否能响应事件，还要判断点击位置是否落在自身的范围内，那该如何判断？ 这就是说到 `hitTest:withEvent:` 和 `pointInside:withEvent:`

### 0x03 `hitTest:withEvent:`

在说 `hitTest:withEvent:` 前，要先知道若视图满足下面的任一条件，视图都不能响应事件：

- `userInteractionEnabled = NO`

- `hidden = YES` 如果父视图隐藏，那么子视图也会隐藏，隐藏的视图无法接收事件
- alpha < 0.01 如果设置一个视图的透明度 < 0.01，会直接影响子视图的透明度。alpha：0.0 ~ 0.01为透明

每个 `UIView` 对象都有一个 `hitTest:withEvent:` 方法，这是查找第一响应者过程中最核心的存在。其作用是询问事件在当前视图中的响应者，同时又是作为事件传递的桥梁。

`hitTest:withEvent:` 方法返回一个 `UIView` 对象，作为当前视图层次中的响应者。默认实现是：

- 若当前视图无法响应事件，则返回 `nil`

- 若当前视图可以响应事件，但无子视图可以响应事件，则返回自身作为当前视图层次中的事件响应者
- 若当前视图可以响应事件，同时有子视图可以响应，则返回子视图层次中的事件响应者。

```Objective-C
- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event{
    // 3种状态无法响应事件
    if (self.userInteractionEnabled == NO || self.hidden == YES ||  self.alpha <= 0.01) return nil; 
    // 触摸点若不在当前视图上则无法响应事件
    if ([self pointInside:point withEvent:event] == NO) return nil; 
    // 从后往前遍历子视图数组 
    int count = (int)self.subviews.count; 
    for (int i = count - 1; i >= 0; i--) 
    { 
        // 获取子视图
        UIView *childView = self.subviews[i]; 
        // 坐标系的转换,把触摸点在当前视图上坐标转换为在子视图上的坐标
        CGPoint childP = [self convertPoint:point toView:childView]; 
        // 询问子视图层级中的最佳响应视图
        UIView *fitView = [childView hitTest:childP withEvent:event]; 
        if (fitView) 
        {
            // 如果子视图中有更合适的就返回
            return fitView; 
        }
    } 
    // 没有在子视图中找到更合适的响应视图，那么自身就是最合适的
    return self;
}
```

现在我们在上述示例的视图层次中的每个视图类中添加如下 3 个方法来验证一下之前的分析, 以 `B1View` 为例

```Objective-C
@implementation B1View

- (UIView *)hitTest:(CGPoint)point withEvent:(UIEvent *)event
{
    NSLog(@"%s", __func__);
    return [super hitTest:point withEvent:event];
}

- (BOOL)pointInside:(CGPoint)point withEvent:(UIEvent *)event
{
    NSLog(@"%s", __func__);
    return [super pointInside:point withEvent:event];
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    NSLog(@"%s---%@", __func__, touches);
}

@end
```

点击 `View B1` 函数调用栈如下：

```
-[CView hitTest:withEvent:]
-[CView pointInside:withEvent:]
-[BView hitTest:withEvent:]
-[BView pointInside:withEvent:]
-[B2View hitTest:withEvent:]
-[B2View pointInside:withEvent:]
-[B1View hitTest:withEvent:]
-[B1View pointInside:withEvent:]

-[CView hitTest:withEvent:]
-[CView pointInside:withEvent:]
-[BView hitTest:withEvent:]
-[BView pointInside:withEvent:]
-[B2View hitTest:withEvent:]
-[B2View pointInside:withEvent:]
-[B1View hitTest:withEvent:]
-[B1View pointInside:withEvent:]
-[B1View touchesBegan:withEvent:]
```

可以看到最终是视图 `View B1` 先对事件进行了响应，同时事件传递过程也和之前的分析一致。事实上单击后从 `[CView hitTest:withEvent:]` 到 [B1View pointInside:withEvent:] 的过程会执行两遍，两次传的是同一个 touch，区别在于 touch 的状态不同，第一次是 begin 阶段，第二次是 end 阶段。也就是说，应用对于事件的传递起源于触摸状态的变化。

<br>

## 四、事件响应

### 0x01 事件响应的前奏

因为最佳响应者具有最高的事件响应优先级，因此 UIApplication 会先将事件传递给它供其响应。首先，UIApplication 将事件通过 `sendEvent:` 传递给事件所属的 window，window 同样通过 `sendEvent:` 再将事件传递给最佳响应者视图。过程如下

```
UIApplication ——> UIWindow ——> hit-tested view
```

以点击 `View B1` 为例，在 `touchesBegan:withEvent:` 中下断点，得到函数调用栈如下：

```Objective-C
* thread #1, queue = 'com.apple.main-thread', stop reason = breakpoint 1.1
  * frame #0: 0x00000001008b94f8 ZZTouchesEvents`-[B1View touchesBegan:withEvent:](self=0x0000000100d117e0, _cmd="touchesBegan:withEvent:", touches=1 element, event=0x000000028151c000) at B1View.m:26:33
    frame #1: 0x000000019830d448 UIKitCore`-[UIWindow _sendTouchesForEvent:] + 496
    frame #2: 0x000000019830ef9c UIKitCore`-[UIWindow sendEvent:] + 3976
    frame #3: 0x00000001982e8bc0 UIKitCore`-[UIApplication sendEvent:] + 712
    frame #4: 0x0000000198370118 UIKitCore`__dispatchPreprocessedEventFromEventQueue + 7360
    frame #5: 0x0000000198373070 UIKitCore`__processEventQueue + 6460
    frame #6: 0x000000019836a5f4 UIKitCore`__eventFetcherSourceCallback + 160
    frame #7: 0x000000019592381c CoreFoundation`__CFRUNLOOP_IS_CALLING_OUT_TO_A_SOURCE0_PERFORM_FUNCTION__ + 28
    frame #8: 0x0000000195923718 CoreFoundation`__CFRunLoopDoSource0 + 208
    frame #9: 0x0000000195922a28 CoreFoundation`__CFRunLoopDoSources0 + 268
    frame #10: 0x000000019591cd20 CoreFoundation`__CFRunLoopRun + 824
    frame #11: 0x000000019591c4bc CoreFoundation`CFRunLoopRunSpecific + 600
    frame #12: 0x00000001ac42e820 GraphicsServices`GSEventRunModal + 164
    frame #13: 0x00000001982c9164 UIKitCore`-[UIApplication _run] + 1072
    frame #14: 0x00000001982ce840 UIKitCore`UIApplicationMain + 168
    frame #15: 0x00000001008b9ba0 ZZTouchesEvents`main(argc=1, argv=0x000000016f54b888) at main.m:17:12
    frame #16: 0x00000001955e3e40 libdyld.dylib`start + 4
```

那么问题又来了。这个过程中，假如应用中存在多个 window 对象，UIApplication 是怎么知道要把事件传给哪个 window 的？window 又是怎么知道哪个视图才是最佳响应者的呢？

其实简单思考一下，这两个过程都是传递事件的过程，涉及的方法都是 `sendEvent:` ，而该方法的参数（`UIEvent` 对象）是唯一贯穿整个经过的线索，那么就可以大胆猜测必然是该触摸事件对象上绑定了这些信息。事实上之前在介绍 `UITouch` 的时候就说过 touch 对象保存了触摸所属的 window 及 view，而 event 对象又绑定了 touch 对象，如此一来，是不是就说得通了。要是不信的话，那就自定义一个 `ZZWindow` 类，重写 `sendEvent:`方法，捕捉该方法调用时参数 event 的状态，答案就显而易见了

```Objective-C
@implementation ZZWindow

- (void)sendEvent:(UIEvent *)event
{
    [super sendEvent:event];
}

@end
```

然后在 `sendEvent:` 方法中下断点，查看 `event ` 中信息

![](../Images/iOS/TouchesEvents/TouchesEvents_04.png)

![](../Images/iOS/TouchesEvents/TouchesEvents_05.png)

至于这两个属性是什么时候绑定到 touch 对象上的，必然是在查找第一响应者的过程中。

### 0x02 事件响应链 (Responder Chain)

前面提到的最佳响应者，之所以称之为“最佳”，是因为其具备响应事件的最高优先权（响应链顶端的男人）。最佳响应者首先接收到事件，然后便拥有了对事件的绝对控制权：即它可以选择处理这个事件，也可以将这个事件转发给其他响应者 (`nextResponder `)，这个由响应者构成的视图链就称之为响应链。

**响应者对于事件的操作方式：**

响应者对于事件的拦截以及传递都是通过 `touchesBegan:withEvent:` 方法控制的，该方法的默认实现是将事件沿着默认的响应链往下传递。

响应者对于接收到的事件有3种操作：

- 不拦截，默认操作。事件会自动沿着默认的响应链往下传递

- 拦截，不再往下分发事件。重写 `touchesBegan:withEvent:` 进行事件处理，不调用父类的 `touchesBegan:withEvent:`
- 拦截，继续往下分发事件
重写 `touchesBegan:withEvent:` 进行事件处理，同时调用父类的 `touchesBegan:withEvent:` 将事件往下传递

**响应链中的事件传递规则：**

每一个响应者对象（`UIResponder` 对象）都有一个 `nextResponder` 方法，用于获取响应链中当前对象的下一个响应者。因此，一旦事件的最佳响应者确定了，这个事件所处的响应链就确定了。

对于响应者对象，默认的 `nextResponder` 实现如下：

- `UIView` 对象：若视图是控制器的根视图，则其`nextResponder`为控制器对象；否则，其`nextResponder`为父视图。

- `UIViewController` 对象：

	- 若控制器的视图是 `window` 的根视图，则其 `nextResponder` 为 `UIWindow` 对象；
	
	- 若控制器是从别的控制器 presented 出来的，则其 `nextResponder` 为 presenting view controller。

- `UIWindow` 对象： `nextResponder` 为 `UIApplication` 对象。

- `UIApplication` 对象：若当前应用的 `AppDelegate` 是一个 `UIResponder` 对象，且不是 `UIView`、`UIViewController` 或 app 本身，则 `UIApplication` 的 `nextResponder` 为 `AppDelegate`。

![](../Images/iOS/TouchesEvents/TouchesEvents_06.png)

如上取官网一张图，若点击在 `UITextField ` 则时间的传递顺序如下：

```
UITextField ——> UIView ——> UIView ——> UIViewController ——> UIWindow ——> UIApplication ——> UIApplicationDelegate
```

图中虚线箭头是指若该 `UIView` 是作为 `UIViewController` 根视图存在的，则其 `nextResponder` 为 `UIViewController` 对象；若是直接 `addSubview:` 在 `UIWindow` 上的，则其 `nextResponder` 为 `UIWindow` 对象。

定义 `printResponderChain` 方法打印响应链，如下

```Objective-C
- (void)printResponderChain
{
    UIResponder *responder = self;
    printf("%s",[NSStringFromClass([responder class]) UTF8String]);
    while (responder.nextResponder) {
        responder = responder.nextResponder;
        printf(" --> %s",[NSStringFromClass([responder class]) UTF8String]);
    }
}
```

然后在 `B1View` 的 `touchesBegan:withEvent:` 中调用，如下

```Objective-C
- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    NSLog(@"%s---%@", __func__, touches);
    [self printResponderChain];
}
```

得到如下结果

```Objective-C
B1View --> BView --> UIView --> ViewController --> UIDropShadowView 
--> UITransitionView --> ZZWindow --> UIWindowScene --> UIApplication --> AppDelegate
```

<br>


<br>

**Reference**

- [iOS触摸事件全家桶](https://www.jianshu.com/p/c294d1bd963d)

- [Touches, Presses, and Gestures](https://developer.apple.com/documentation/uikit/touches_presses_and_gestures)

- [事件响应机制](https://yeziahehe.com/2020/01/19/responder_chain/)

- [事件传递和响应链（Responder Chain）](https://github.com/pro648/tips/blob/master/sources/%E4%BA%8B%E4%BB%B6%E4%BC%A0%E9%80%92%E5%92%8C%E5%93%8D%E5%BA%94%E9%93%BE%EF%BC%88Responder%20Chain%EF%BC%89.md)
<br>