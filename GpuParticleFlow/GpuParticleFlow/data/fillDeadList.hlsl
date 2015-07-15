AppendStructuredBuffer<uint> deadList;

[numthreads(1024, 1,1)]
void CSMain(uint3 id : SV_DispatchThreadID)
{
	deadList.Append(id.x);
}