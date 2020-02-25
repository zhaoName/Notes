
# App优化 - 电量优化


如果有一天有人突然告诉你，app的电量消耗非常快。你的第一反应可能是

- 后台持续定位

- 后台循环播放无声音乐

- 后台持续下载

- 频繁网络请求

等各种原因造成的，上述这些原因都能很快的定位并解决掉。还有些是由于子线程有很大性能消耗造成的，而子线程可能是你自己创建的，也可能是第三方创建，我们需要把它找出来。

### 0x01 手机电量获取

- 最简单的方式是调用`UIDevice`中方法获取

```
[UIDevice currentDevice].batteryLevel
```

- 也可以使用`IOKit`中的方法

`IOKit`需要你自己去下载，导入到工程

```
#import "IOPSKeys.h"
#import "IOPowerSources.h"

+ (double)getBatteryLevel
{
    // 返回电量信息
    CFTypeRef blob = IOPSCopyPowerSourcesInfo();
    // 返回电量句柄列表数据 CFArrayRef = [CFTypeRef, CFTypeRef, ...]
    CFArrayRef sources = IOPSCopyPowerSourcesList(blob);
    CFDictionaryRef pSource = NULL;
    const void *psValue;
    // 返回数组大小
    CFIndex numOfSources = CFArrayGetCount(sources);
    // 计算大小出错处理
    if (numOfSources == 0) {
        NSLog(@"Error in CFArrayGetCount");
        CFRelease(sources);
        CFRelease(blob);
        return -1.0f;
    }
    /* source
     {
         "Battery Provides Time Remaining" = 1;
         BatteryHealth = "Check Battery";
         "Current Capacity" = 65;
         "Is Charging" = 1;
         "Is Finishing Charge" = 0;
         "Is Present" = 1;
         "Max Capacity" = 100;
         Name = "InternalBattery-0";
         "Power Source ID" = 2293859;
         "Power Source State" = "AC Power";
         "Raw External Connected" = 1;
         "Show Charging UI" = 1;
         "Time to Empty" = 0;
         "Time to Full Charge" = 0;
         "Transport Type" = Internal;
         Type = InternalBattery;
     }*/
    for (int i=0; i<numOfSources; i++) {
        pSource = IOPSGetPowerSourceDescription(blob, CFArrayGetValueAtIndex(sources, i));
        if (!pSource) {
            NSLog(@"Error in IOPSGetPowerSourceDescription");
            CFRelease(sources);
            CFRelease(blob);
            return -1.0f;
        }
        // kIOPSNameKey: the current power source's name.
        psValue = CFDictionaryGetValue(pSource, CFSTR(kIOPSNameKey));
        NSLog(@"%s", psValue);
        
        int curCapacity = 0;
        int maxCapacity = 0;
        double percentage;
        
        // kIOPSCurrentCapacityKey: the current power source's maximum or "Full Charge Capacity"
        psValue = CFDictionaryGetValue(pSource, CFSTR(kIOPSCurrentCapacityKey));
        CFNumberGetValue((CFNumberRef)psValue, kCFNumberSInt32Type, &curCapacity);
        
        // kIOPSMaxCapacityKey: the current power source's design capacity
        psValue = CFDictionaryGetValue(pSource, CFSTR(kIOPSMaxCapacityKey));
        CFNumberGetValue((CFNumberRef)psValue, kCFNumberSInt32Type, &maxCapacity);

        percentage = ((double) curCapacity / (double) maxCapacity * 100.0f);
        NSLog(@"curCapacity : %d / maxCapacity: %d , percentage: %.1f ", curCapacity, maxCapacity, percentage);
        return percentage;
    }
    
    CFRelease(sources);
    CFRelease(blob);
    return 0.0;
}
```


<br>

### 0x02 CPU 使用率

 `task_threads()` 函数能够得到所有的线程信息数组、线程总数。线程数组里的线程信息结构体`thread_basic_info` 里有一个记录 CPU 使用百分比的字段 `cpu_usage`。`thread_basic_info` 结构体的代码如下：


```
struct thread_basic_info {
	time_value_t    user_time;      /* user run time */
	time_value_t    system_time;    /* system run time */
	integer_t       cpu_usage;      /* scaled cpu usage percentage */
	policy_t        policy;         /* scheduling policy in effect */
	integer_t       run_state;      /* run state (see below) */
	integer_t       flags;          /* various flags (see below) */
	integer_t       suspend_count;  /* suspend count for thread */
	integer_t       sleep_time;     /* number of seconds that thread
	                                 *  has been sleeping */
};
```

如果某个线程的 CPU 的使用率超过一直较高，就可以认为它出问题了。这时将其方法堆栈记录下来，你就可以知道到底是哪段代码让你 App 的电量消耗多了。

```
#include <mach/mach_types.h>
#include <mach/task.h>
#include <mach/mach_init.h>
#include <mach/thread_act.h>

+ (void)updateCPU
{
    thread_act_array_t threads;
    mach_msg_type_number_t threadCount = 0;
    const task_t thisTask = mach_task_self();
    kern_return_t kr = task_threads(thisTask, &threads, &threadCount);
    
    if (kr != KERN_SUCCESS) {
        return;
    }
    for (int i=0; i<threadCount; i++)
    {
        thread_info_data_t threadInfo;
        thread_basic_info_t threadBaseInfo;
        mach_msg_type_number_t threadInfoCount = THREAD_INFO_MAX;
        // typedef thread_act_t  *thread_act_array_t;
        if (thread_info((thread_act_t)threads[i], THREAD_BASIC_INFO, threadInfo, &threadInfoCount) == KERN_SUCCESS) {
            threadBaseInfo = (thread_basic_info_t)threadInfo;
            integer_t cpuUsage = threadBaseInfo->cpu_usage;
            if (cpuUsage > 90) {
                //cup 消耗大于 90 时打印和记录堆栈
                // 这是戴铭自己写的方法
                NSString *reStr = smStackOfThread(threads[i]);
                // 记录数据库中
                NSLog(@"CPU useage overload thread stack：\n%@",reStr);
            }
        }
    }
}
```

对 CPU 的使用要精打细算，要避免让 CPU 做多余的事情。对于大量数据的复杂计算，应该把数据传到服务器去处理，如果必须要在 App 内处理复杂数据计算，可以通过 GCD 的 `dispatch_block_create_with_qos_class` 方法指定队列的 Qos 为 `QOS_CLASS_UTILITY`，将计算工作放到这个队列的 block 里。在 `QOS_CLASS_UTILITY` 这种 Qos 模式下，系统针对大量数据的计算，以及复杂数据处理专门做了电量优化。

<br>

### 0x03 I/O 操作

业内的普遍做法是，将碎片化的数据磁盘存储操作延后，先在内存中聚合，然后再进行磁盘存储。碎片化的数据进行聚合，在内存中进行存储的机制，可以使用系统自带的 NSCache 来完成。


具体可参考 SDWebImage 中关于将图片缓存到内存的做法

```
- (UIImage *)imageFromMemoryCacheForKey:(NSString *)key {
    return [self.memCache objectForKey:key];
}
 
- (UIImage *)imageFromDiskCacheForKey:(NSString *)key {
    // 检查 NSCache 里是否有
    UIImage *image = [self imageFromMemoryCacheForKey:key];
    if (image) {
        return image;
    }
    // 从磁盘里读
    UIImage *diskImage = [self diskImageForKey:key];
    if (diskImage && self.shouldCacheImagesInMemory) {
        NSUInteger cost = SDCacheCostForImage(diskImage);
        [self.memCache setObject:diskImage forKey:key cost:cost];
    }
    return diskImage;
}
```


<br>

`参考`

- [深入剖析 iOS 性能优化](http://www.starming.com/2017/06/20/deeply-ios-performance-optimization/)

- [mach_init](https://opensource.apple.com/source/Libc/Libc-186/mach.subproj/mach_init.c)

- [Energy Efficiency Guide for iOS Apps](https://developer.apple.com/library/archive/documentation/Performance/Conceptual/EnergyGuide-iOS/NotificationBestPractices.html#//apple_ref/doc/uid/TP40015243-CH26-SW1)