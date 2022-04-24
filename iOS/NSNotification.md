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

## 二、GSIMapTable

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

### 0x03 `buckets`

hash 算出对应 `bucket` 在 `buckets` 的下标

```Objective-C
GS_STATIC_INLINE GSIMapBucket
GSIMapPickBucket(unsigned hash, GSIMapBucket buckets, uintptr_t bucketCount)
{
    return buckets + hash % bucketCount;
}

GS_STATIC_INLINE GSIMapBucket
GSIMapBucketForKey(GSIMapTable map, GSIMapKey key)
{
    return GSIMapPickBucket(GSI_MAP_HASH(map, key), map->buckets, map->bucketCount);
}

```

将 `node` 添加到 `map` 中

```Objective-C
/// insert `node` to bucket (头插法 )
GS_STATIC_INLINE void GSIMapLinkNodeIntoBucket(GSIMapBucket bucket, GSIMapNode node)
{
    node->nextInBucket = bucket->firstNode;
    bucket->firstNode = node;
}

/// insert `node` to bucket
GS_STATIC_INLINE void GSIMapAddNodeToBucket(GSIMapBucket bucket, GSIMapNode node)
{
    GSIMapLinkNodeIntoBucket(bucket, node);
    bucket->nodeCount += 1;
}

/// add `node` to map
GS_STATIC_INLINE void GSIMapAddNodeToMap(GSIMapTable map, GSIMapNode node)
{
    GSIMapBucket    bucket;
    // node->key 散列函数算出 node 所在的 bucket
    bucket = GSIMapBucketForKey(map, node->key);
    // 将 node 添加到对应的 bucket
    GSIMapAddNodeToBucket(bucket, node);
    map->nodeCount++;
}
```

从 `map` 中删除对应的 `node`


```Objective-C
/// delete `node` form bucket
GS_STATIC_INLINE void GSIMapUnlinkNodeFromBucket(GSIMapBucket bucket, GSIMapNode node)
{
    if (node == bucket->firstNode)
    {
        bucket->firstNode = node->nextInBucket;
    }
    else
    {
        GSIMapNode	tmp = bucket->firstNode;
        
        while (tmp->nextInBucket != node)
        {
            tmp = tmp->nextInBucket;
        }
        tmp->nextInBucket = node->nextInBucket;
    }
    node->nextInBucket = 0;
}


/// delete `node` from bucket
GS_STATIC_INLINE void GSIMapRemoveNodeFromBucket(GSIMapBucket bucket, GSIMapNode node)
{
    bucket->nodeCount--;
    GSIMapUnlinkNodeFromBucket(bucket, node);
}

/// delete `node` from map
GS_STATIC_INLINE void
GSIMapRemoveNodeFromMap(GSIMapTable map, GSIMapBucket bkt, GSIMapNode node)
{
    map->nodeCount--;
    GSIMapRemoveNodeFromBucket(bkt, node);
}
```

删除并释放 `node`

```Objective-C
GS_STATIC_INLINE void GSIMapFreeNode(GSIMapTable map, GSIMapNode node)
{
    GSI_MAP_RELEASE_KEY(map, node->key);
    GSI_MAP_CLEAR_KEY(node);
#if	GSI_MAP_HAS_VALUE
    GSI_MAP_RELEASE_VAL(map, node->value);
    GSI_MAP_CLEAR_VAL(node);
#endif
    
    // 将待释放的 node 插入到 freeNodes 中 (头插法)
    node->nextInBucket = map->freeNodes;
    map->freeNodes = node;
}

GS_STATIC_INLINE GSIMapNode
GSIMapRemoveAndFreeNode(GSIMapTable map, uintptr_t bkt, GSIMapNode node)
{
    GSIMapNode next = node->nextInBucket;
    // delete node from map
    GSIMapRemoveNodeFromMap(map, &(map->buckets[bkt]), node);
    // free node that delete node from map
    GSIMapFreeNode(map, node);
    return next;
}
```

根据 `key` 在 `map` 中查找对应的 `node`

```Objective-C
/// lookup node from bucket
GS_STATIC_INLINE GSIMapNode
GSIMapNodeForKeyInBucket(GSIMapTable map, GSIMapBucket bucket, GSIMapKey key)
{
    GSIMapNode    node = bucket->firstNode;
    
    if (GSI_MAP_ZEROED(map))
    {
        while ((node != 0)
               && GSI_MAP_EQUAL(map, GSI_MAP_READ_KEY(map, &node->key), key) == NO)
        {
            GSIMapNode    tmp = node->nextInBucket;
            
            if (GSI_MAP_NODE_IS_EMPTY(map, node))
            {
                GSIMapRemoveNodeFromMap(map, bucket, node);
                GSIMapFreeNode(map, node);
            }
            node = tmp;
        }
        return node;
    }
    while ((node != 0)
           && GSI_MAP_EQUAL(map, GSI_MAP_READ_KEY(map, &node->key), key) == NO)
    {
        node = node->nextInBucket;
    }
    return node;
}

/// lookup node from map table
GS_STATIC_INLINE GSIMapNode GSIMapNodeForKey(GSIMapTable map, GSIMapKey key)
{
    GSIMapBucket    bucket;
    GSIMapNode    node;
    
    if (map->nodeCount == 0)
    {
        return 0;
    }
    bucket = GSIMapBucketForKey(map, key);
    node = GSIMapNodeForKeyInBucket(map, bucket, key);
    return node;
}
```

<br>

## 三、NSNotificationCenter

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