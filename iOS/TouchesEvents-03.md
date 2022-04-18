# 触摸事件的实践、应用

本文主要介绍触摸事件的实战、应用

<br>

### 0x01 改变可点击区域

e.g.1 有时给的图标较小，放在 `UIButton` 上导致其点击区域较小，不太方便用户点击。如下：

![](../Images/iOS/TouchesEvents/TouchesEvents_0301.png)

由前面所学可知，当超出`UIButton` 的区域时，`pointInside:withEvent:` 会返回 `NO`。导致点击事件不能被响应。 这时若要扩大 `UIButton` 点击范围，只需要在 `pointInside:withEvent:` 方法中操作既可以。

如下将 `ZZExpandButton ` 的点击区域上下左右都扩大 20。

```Objective-C
@implementation ZZExpandButton

- (BOOL)pointInside:(CGPoint)point withEvent:(UIEvent *)event
{
    CGRect touchRect =  CGRectInset(self.bounds, -20, -20);
    return CGRectContainsPoint(touchRect, point);
}

@end
```

<br>

e.g.2 还有种情况是点击范围超出父视图的范围，这样点击事件在父视图的 `pointInside:withEvent:` 就会被拦截。

如下图，当点击 `ZZExpandButton` 的上半截区域是不响应事件的。

![](../Images/iOS/TouchesEvents/TouchesEvents_0302.png)


这时需要在父视图的 `pointInside:withEvent:`  方法中判断，若点击在 `ZZExpandButton ` 也要返回 `YES`

```Objective-C
@implementation ZZExpandView

- (BOOL)pointInside:(CGPoint)point withEvent:(UIEvent *)event
{
    CGPoint btnPoint = [self convertPoint:point toView:subBtn];
    if ([subBtn pointInside:btnPoint withEvent:event]) {
        return  YES;
    }
    return [super pointInside:point withEvent:event];
}

@end
```

<br>

### 0x02 

<br>


<br>



