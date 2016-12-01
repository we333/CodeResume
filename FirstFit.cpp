#include <iostream>
#include <list> 
#include <limits.h>	// for UINT_MAX
using namespace std;

struct MemoryBlock
{
	unsigned int m_addr;
	unsigned int m_size;
	MemoryBlock() : m_addr( 0 ), m_size( 0 ){}
	MemoryBlock( unsigned int addr, unsigned int size ) : m_addr( addr ), m_size( size ){}
};

list<MemoryBlock> freeBlockList;

bool ObtainBlock( list<MemoryBlock>& freeBlockList, unsigned int size )
{
	list<MemoryBlock>::iterator it = freeBlockList.begin();
	list<MemoryBlock>::iterator end = freeBlockList.end();

	for(; it != end; it++)
	{
		unsigned int useful = (*it).m_size - (*it).m_addr;
		if(useful >= size)
		{
			(*it).m_addr += size;
			if((*it).m_addr == (*it).m_size)	// 此listNode被用完
				freeBlockList.erase(it);
			return true;
		}
	}
	return false;
}

bool RestoreBlock( list<MemoryBlock>& freeBlockList, unsigned int addr, unsigned int size )
{
	list<MemoryBlock>::iterator it;
	list<MemoryBlock>::iterator begin = freeBlockList.begin();
	list<MemoryBlock>::iterator end = freeBlockList.end();
cout<<"Restore: addr ="<<addr<<"\tsize = "<<size<<endl;

	for(it = begin; it != end; it++)
	{
		MemoryBlock tmp = *it;
		if(addr >= tmp.m_addr && addr < tmp.m_size)					//	addr [m_addr, m_size)
			return false;
		if(addr + size > tmp.m_addr && addr + size <= tmp.m_size)	//	addr+size (m_addr, m_size]							// 
			return false;
		if(tmp.m_addr >= addr && tmp.m_size <= addr+size)			
			return false;
	}

	for(it = begin; it != end; it++)
		if((*it).m_addr >= addr) 	// 找到离释放内存最近的listNode
			break;

//	获取紧邻待释放区域的前后一个ListNode
	MemoryBlock right = *it;
	it--;
	MemoryBlock left = *it;
	it++;	// it == right

cout<<"left: addr = "<<left.m_addr<<"\tsize = "<<left.m_size<<endl;
cout<<"right: addr = "<<right.m_addr<<"\tsize = "<<right.m_size<<endl;

	if(left.m_size == addr)					// 左边紧挨着一个listNode
	{
		if(right.m_addr == addr + size)		// 右边也紧挨着一个listNode
		{
cout<<"____///___"<<endl;
			it--;
			(*it).m_size = right.m_size;		// 左中右合并为左,删除右
			freeBlockList.erase(++it);
			return true;
		}
		else
		{
cout<<"____///***"<<endl;
			it--;
			(*it).m_size += size;
			return true;
		}
	}
	else									// 左边没有挨着listNode
	{
		if(right.m_addr == addr + size)		// 右边仍紧挨着一个listNode
		{
cout<<"***///___"<<endl;
			(*it).m_addr = addr;			// 更新右的addr即可
			return true;
		}
		else								// 左右都没有挨着listNode
		{
cout<<"***///***"<<endl;
			MemoryBlock mb;					
        	mb.m_addr = addr; mb.m_size = addr + size;
			freeBlockList.insert(it, mb);
			return true;
		}
	}
}

void print_list(list<MemoryBlock>& freeBlockList)
{
cout<<"result-----------------------"<<endl;
	list<MemoryBlock>::iterator it = freeBlockList.begin();
	list<MemoryBlock>::iterator end = freeBlockList.end();
	for(; it != end; it++)
	{
		cout<<"addr = "<<(*it).m_addr<<"\t";
		cout<<"size = "<<(*it).m_size<<endl;
	}
}

void Obtain(int size)
{
	ObtainBlock( freeBlockList, size );
}

void Restore(int addr, int size)
{
	RestoreBlock( freeBlockList, addr, size );
}

int main()
{
	
	MemoryBlock mb = MemoryBlock(0, 4294967295);

	freeBlockList.push_back(mb);
	

	Obtain( 10 );
	Obtain( 10 );
	Obtain( 10 );
	Obtain( 10 );
	Restore( 20, 10 );
	Restore( 0, 10 );
	Obtain( 25 );
/**/
print_list(freeBlockList);

	return 0;
}