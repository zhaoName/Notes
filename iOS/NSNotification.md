# NSNotification


<br>

## 一、NSNotification

`NSNotification` 包含名称、对象、可选字典。

- 名称：`NSNotification` 的名称

- 对象：通知的发布者想要发送给该通知的观察者的任何对象（通常是发布通知的对象）
- 可选字典：可能包含有关通知的附加信息

定义如下：

```Objective-C
@interface NSNotification : NSObject <NSCopying, NSCoding>

@property (readonly, copy) NSNotificationName name;
@property (nullable, readonly, retain) id object;
@property (nullable, readonly, copy) NSDictionary *userInfo;

- (instancetype)initWithName:(NSNotificationName)name object:(nullable id)object userInfo:(nullable NSDictionary *)userInfo API_AVAILABLE(macos(10.6), ios(4.0), watchos(2.0), tvos(9.0)) NS_DESIGNATED_INITIALIZER;
- (nullable instancetype)initWithCoder:(NSCoder *)coder NS_DESIGNATED_INITIALIZER;

@end


@interface NSNotification (NSNotificationCreation)

+ (instancetype)notificationWithName:(NSNotificationName)aName object:(nullable id)anObject;
+ (instancetype)notificationWithName:(NSNotificationName)aName object:(nullable id)anObject userInfo:(nullable NSDictionary *)aUserInfo;

- (instancetype)init /*API_UNAVAILABLE(macos, ios, watchos, tvos)*/;	/* do not invoke; not a valid initializer for this class */

@end
```

> You don’t usually create your own notifications directly, but instead call the `NSNotificationCenter` methods `postNotificationName:object:` and `postNotificationName:object:userInfo:`.

官方文档有句话表明，我们经常使用 `NSNotificationCenter` 的 `postNotificationName:object:` 和 `postNotificationName:object:userInfo:` 来间接创建 `NSNotification` 对象，而不是调用它的初始化方法。

<br>

## 二、NSNotificationCenter

### 0x01


```Objective-C
#define	CHUNKSIZE	128
#define	CACHESIZE	16

typedef struct NCTbl {
    Observation		*wildcard;	/* Get ALL messages.		*/
    GSIMapTable		nameless;	/* Get messages for any name.	*/
    GSIMapTable		named;		/* Getting named messages only.	*/
    unsigned		lockCount;	/* Count recursive operations.	*/
    NSRecursiveLock	*_lock;		/* Lock out other threads.	*/
    Observation		*freeList;
    Observation		**chunks;
    unsigned		numChunks;
    GSIMapTable		cache[CACHESIZE];
    unsigned short	chunkIndex;
    unsigned short	cacheIndex;
} NCTable;
```



```Objective-C
/*
 * Observation structure - One of these objects is created for
 * each -addObserver... request.  It holds the requested selector,
 * name and object.  Each struct is placed in one LinkedList,
 * as keyed by the NAME/OBJECT parameters.
 * If 'next' is 0 then the observation is unused (ie it has been
 * removed from, or not yet added to  any list).  The end of a
 * list is marked by 'next' being set to 'ENDOBS'.
 *
 * This is normally a structure which handles memory management using a fast
 * reference count mechanism, but when built with clang for GC, a structure
 * can't hold a zeroing weak pointer to an observer so it's implemented as a
 * trivial class instead ... and gets managed by the garbage collector.
 */

typedef	struct	Obs {
    id		observer;	/* Object to receive message.	*/
    SEL		selector;	/* Method selector.		*/
    struct Obs	*next;		/* Next item in linked list.	*/
    int		retained;	/* Retain count for structure.	*/
    struct NCTbl	*link;		/* Pointer back to chunk table	*/
} Observation;
```

### 0x01 `_GSIMapBucket`

`_GSIMapBucket` 的定义如下：

```Objective-C
struct	_GSIMapNode {
    GSIMapNode nextInBucket;	/* Linked list of bucket.	*/
    GSIMapKey key;
#if	GSI_MAP_HAS_VALUE
    GSIMapVal	value;
#endif
};

struct	_GSIMapBucket {
    uintptr_t nodeCount;	/* Number of nodes in bucket.	*/
    GSIMapNode firstNode;	/* The linked list of nodes.	*/
};
```

- `_GSIMapBucket` 是一个链表，`firstNode` 是其第一个节点。

- `_GSIMapNode` 是节点的类型。其成员变量 `key` 用于 hash，可算出当前 `_GSIMapBucket` 在数组中的下标。

还需关注两个宏定义，如下：

```Objective-C
#if	!defined(GSI_MAP_TABLE_T)
typedef struct _GSIMapBucket GSIMapBucket_t;
typedef struct _GSIMapNode GSIMapNode_t;

typedef GSIMapBucket_t *GSIMapBucket;
typedef GSIMapNode_t *GSIMapNode;
#endif
```

- `GSIMapBucket` 可看做数组，成员为 `_GSIMapBucket` 类型

- `GSIMapNode` 可看做数组，成员为 `_GSIMapNode ` 类型


### 0x02 `_GSIMapTable `

`_GSIMapTable` 定义如下：
 

```Objective-C
typedef struct _GSIMapTable GSIMapTable_t;
typedef GSIMapTable_t *GSIMapTable;

struct	_GSIMapTable {
    NSZone	*zone;
    uintptr_t nodeCount;	/* Number of used nodes in map.	*/
    uintptr_t bucketCount;	/* Number of buckets in map.	*/
    // 其实是个散列表，使用 node->key 快速算出 node 对应的 bucket 在 buckets 的下标
    GSIMapBucket buckets;	/* Array of buckets.		*/
    GSIMapNode	freeNodes;	/* List of unused nodes.	*/
    uintptr_t chunkCount;	/* Number of chunks in array.	*/
    GSIMapNode *nodeChunks;	/* Chunks of allocated memory.	*/
    uintptr_t increment;
#ifdef	GSI_MAP_EXTRA
    GSI_MAP_EXTRA	extra;
#endif
};
```

- `buckets` 其实是个散列表，其成员是链表。使用 `node->key` 快速算出 `node` 对应的 `bucket` 在 `buckets` 中的位置

- `nodeChunks` 可看做二维数组 `[[_GSIMapNode, ...], [_GSIMapNode, ...], ...]`
- `freeNodes` 是个单向连表，存储要释放的元素

所以 `_GSIMapTable` 的结构如下：

```Objective-C
 *  A rough picture is include below:
 *   
 *  
 *   This is the map                C - array of the buckets
 *   +---------------+             +---------------+
 *   | _GSIMapTable  |      /----->| nodeCount     |  
 *   |---------------|     /       | firstNode ----+--\  
 *   | buckets    ---+----/        | ..........    |  |
 *   | bucketCount  =| size of --> | nodeCount     |  |
 *   | nodeChunks ---+--\          | firstNode     |  |
 *   | chunkCount  =-+\ |          |     .         |  | 
 *   |   ....        || |          |     .         |  |
 *   +---------------+| |          | nodeCount     |  |
 *                    | |          | fistNode      |  | 
 *                    / |          +---------------+  | 
 *         ----------   v                             v
 *       /       +----------+      +---------------------------+ 
 *       |       |  * ------+----->| Node1 | Node2 | Node3 ... | a chunk
 *   chunkCount  |  * ------+--\   +---------------------------+
 *  is size of = |  .       |   \  +-------------------------------+
 *               |  .       |    ->| Node n | Node n + 1 | ...     | another
 *               +----------+      +-------------------------------+
 *               array pointing
 *               to the chunks
```

<br>

## 三、

```Objective-C

```



```Objective-C

```



```Objective-C

```


```Objective-C

```


<br>

**Reference**

- [NSNotification - Apple](https://developer.apple.com/documentation/foundation/nsnotification?language=objc)

- [NSNotificationCenter - GNUStep](https://github.com/gnustep/libs-base/blob/master/Source/NSNotificationCenter.m)

<br>