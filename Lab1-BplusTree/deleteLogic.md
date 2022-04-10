# Delete Logic

Placing the entire pseudocode for the delete functions here not to clog the `.cpp` with comments.

> Note: This implementation treats cousins as siblings.

```
DELETE(k):
  locate leaf n that contains k
  locate n->key[i] in n that contains k
  if not found:
    stop the procedure
  if found:
    erase n->key[i]
  if key[i] is not the last element:
    shift the remaining keys in n located to the right of key[i] one space to the left
  reduce number of n's keys by one
  if the number of keys condition isn't met by n:
    let node p be n->parent
    if the left sibling s exists:
      if it can borrow:
          have n borrow the maximum value key it needs from s
          update n's and s's number of keys
      if it can't:
          merge s and n by moving the remaining keys in n to s
          update s' number of keys
          update s->next as n->next
          delete n
          if n was not the rightmost child:
              shift the children located at its left to the left by 1
          reduce number of children in p by 1
    else if the right sibling s exists:
      if it can borrow:
          have n borrow the minimum value key it needs from s
          update n's and s's number of keys
      if it can't:
          merge s and n by moving the remaining keys in n to s
          delete n
          shift the rest of p's children to the left by 1
          reduce the number of children in s's father
    run internal deletion for p
```

```
INTERNAL DELETE(p):
  if p is null:
    stop the procedure
  reset p's keys accordingly
  if p is the root and the root has one child:
    update the root as its child
    stop the procedure
  while the minimum children condition is not met by p:
    if the left sibling s exists:
        if it can borrow:
            borrow children from s
            decrease the number of children in s by 1
            update s's and p's keys
        if it can't:
            merge p onto s (by passing over the remaining children in p to the left of the already existing children in s)
            increase the number of children in s by the number of children passed over
            let temp be p
            set p as s
            delete temp
            update p's keys
    else if the right sibling s exists:
        if it can borrow:
            borrow children from s
            shift s's children to the left
            reduce the number of children in s by 1
            reset p's and s's keys accordingly
        if it can't:
            merge p onto s (by shifting s's children to the right and positioning the remaining children in p in the blank spaces)
            increase the number of children in s by the number of children passed over
            let temp be p
            set p as s
            delete temp
            update p's keys
    update p as p's father
    run internal deletion for p
```