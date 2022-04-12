# 触摸事件

<br>

## 一、`UITouch`、`UIEvent`、`UIResponder`

### 0x01 `UITouch`



### 0x02 `UIEvent`


### 0x03 `UIResponder`


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
- source0回调内部将触摸事件添加到UIApplication对象的事件队列中。事件出队后，UIApplication开始一个寻找最佳响应者的过程，这个过程又称`hit-testing`，细节将后面详述。
- 寻找到最佳响应者后，接下来的事情便是事件在响应链中的被响应了，关于响应链相关的内容详见。
- 事实上，事件除了被响应者消耗，还能被手势识别器或是 `target-action` 模式捕捉并消耗掉。其中涉及对触摸事件的响应优先级，详见。
- 触摸事件历经坎坷后要么被某个响应对象捕获后释放，要么致死也没能找到能够响应的对象，最终释放。至此触摸事件的使命就算终结了。runloop 若没有其他事件需要处理，也将重归于眠，等待新的事件到来后唤醒。

<br>

## 三、事件传递

每个事件的理想宿命是被能够响应它的对象响应后释放，然而响应者诸多，事件一次只有一个，谁都想把事件抢到自己碗里来，为避免纷争，就得有一个先后顺序，也就是得有一个响应者的优先级。因此这就存在一个寻找事件最佳响应者（又称第一响应者 first responder）的过程，目的是找到一个具备最高优先级响应权的响应对象（the most appropriate responder object），这个过程叫做 Hit-Testing，那个命中的最佳响应者称为 hit-tested view。

### 0x01 事件自下而上传递



![](../Images/iOS/TouchesEvents/TouchesEvents_02.png)

![](../Images/iOS/TouchesEvents/TouchesEvents_03.png)



### 0x02 `hitTest:withEvent:`



<br>

## 四、事件响应


<br>


<br>

**Reference**

- [iOS触摸事件全家桶](https://www.jianshu.com/p/c294d1bd963d)

- [Touches, Presses, and Gestures](https://developer.apple.com/documentation/uikit/touches_presses_and_gestures)

<br>