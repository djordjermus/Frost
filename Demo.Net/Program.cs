using Frost.Net.Sync;
var mx = new Frost.Net.Sync.Semaphore(0, 1);
var mx2 = new Frost.Net.Sync.Semaphore(0, 1);
var result = SyncObject.TryAcquireOne(mx, mx2);
var x = 5;