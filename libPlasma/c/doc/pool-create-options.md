# Pool Options #     {#PoolOptions}

This page details the configuration options available for a pool. These options are provided as a \ref bslaw to the \ref pool_create and/or \ref pool_change_options commands.

## Option Map Keys

<div class="map_table">

| key | type | default | description |
| -- | -- | -- | -- |
| resizable | boolean | true | This enables a newer pool format, which supports a number of new features, including being able to resize the pool after it is created. You should almost always have this enabled. The only reason to ever disable it is if you want to create pools that can be read by very old versions of Plasma (g-speak < v2.0). |
| single-file | boolean | false | This means that the pool will reside in a single file, rather than creating an entire directory for the pool. |
| size^ | unt64 (bytes) | (required) | The size of the pool, in bytes. This includes the space used for the header and table of contents, so the actual amount of space available for proteins will be less. The pool wraps around and deletes older proteins once this limit has been exceeded. |
| toc-capacity | unt64 (proteins) | 0 | The size of the table of contents, in entries (one entry for each protein). The default value, 0, indicates that there should be no table of contents. The table of contents is an optimization which makes random access of proteins within the pool faster. If the number of proteins in the pool exceeds the table of contents size, then the table of contents is "decimated" so that it records only ever other protein, or every 4th protein, etc. |
| stop-when-full^ | boolean | false | If true, then the pool stops allowing deposits once it is full. If false, the default, it will wrap around and delete the oldest proteins as newer ones are deposited. |
| frozen^ | boolean | false | Makes the pool read-only until "frozen" is set to false again. |
| auto-dispose^^ | boolean | false | Causes the pool to be automatically deleted once the last hose to it is closed. This option can only be set with pool_change_options(); it cannot be set when the pool is created. |
| sync^ | boolean | false | Causes the pool to be synced to disk after every deposit. This should eliminate the possiblity of corruption due to power failures, at the expense of performance. Note that on Linux, the data is only synced to the disk controller, not to the platter, so there is still a slight risk of data being lost due to a power failure. |
| flock | boolean | Linux: false<br />OS X: true | When true, Plasma uses flock() for locking operations. When false, Plasma uses System V semaphores. The default is to use semaphores on Linux, and flock() on Mac OS X. Windows always uses Windows mutexes, and is not affected by this option. |
| checksum | boolean | false | When true, a checksum is computed for each protein, and stored in the pool. This should make it easier to detect corruption, at the expense of some performance. |
| mode | string (octal) or int32 | -1 | The UNIX permissions for this pool. May be specified either as an int32, or as a string which is parsed as an octal number. We recommend only using "7" or "0" in each position, since write permission is needed to read from pools, and vice versa, so it isn't really possible to specify read and write permissions separately. This option has no effect on Windows. |
| owner | string (username) or int32 (uid) | -1 | The user which should own this pool. Maybe be specified either as a numeric uid, or as a string which is looked up in the password file. This option has no effect on Windows. |
| group | string (groupname) or int32 (gid) | -1 | The group which should own this pool. Maybe be specified either as a numeric gid, or as a string which is looked up in the group file. This option has no effect on Windows. |

</div>

^ available to \ref pool_change_options  
^^  ONLY available to \ref pool_change_options

