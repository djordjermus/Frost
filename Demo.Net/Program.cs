using Frost.Net.Sync;
var mx1 = new Frost.Net.Sync.Semaphore(2, 4);
var mx2 = new Frost.Net.Sync.Mutex(false);
int i = 0;
bool b = false;

b = SyncObject.TryAcquireAll(mx1, mx2);
b = SyncObject.AcquireAll(mx1, mx2);
i = SyncObject.TryAcquireOne(mx1, mx2);
i = SyncObject.AcquireOne(mx1, mx2);

int x = 54;



