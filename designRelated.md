# 2 phase locking
- select lock the entity block of the table;
- insert lock the entity block of the table;
- update lock the corresponding data block- here to simplify the whole process also lock the table;
- delete also lock the corresponding data block- here to simplify the whole process also lock the table;






# lock manager

- lockTable ---- mapping the block index to the requestedQueue
- each reauestedQueue have linked request
- each request containing a transantionID and lockMode and whether the lock is granted...



# isolation level

- READ_UNCOMMITTED
    - do not add the share lock when read
- READ_COMMITTED 
    - add share lock but was released after the reading is done
- REPEATABLE_READ
    - when the whole process gets commited, release all the lock

