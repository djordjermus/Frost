using Frost.Net;
using Frost.Net.Models;

while (true)
{
	var current = SemanticVersion.GetApiVersion();
	var target = new SemanticVersion()
	{
		Major      = 0,
		Minor      = 8,
		Patch      = 0,
	};
	var result = current.CheckCompatibility(target);
}
