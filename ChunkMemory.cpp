#include <vector>
#include <cassert>

// メモリ領域の大きな塊(チャンク)を扱うための構造体
struct Chunk
{
	unsigned char* 	m_pData;					// まとめて取得したメモリ領域。サイズは FixedAllocator クラスの m_blockSize * m_numBlocks。
	unsigned int	m_firstAvailableBlock;		// チャンクの中で、次に使用できる未使用ブロック
	unsigned int	m_blocksAvailable;			// チャンクの中で、現在の未使用のブロック数

	void 	Init( size_t blockSize, unsigned int blocks );
	void* 	Allocate( size_t blockSize );
	void 	Deallocate( void* p, size_t blockSize );
	void 	Release(){ delete m_pData; }
};

// 複数のチャンクを扱うクラス
class FixedAllocator
{
	size_t			m_blockSize;				// ブロックの大きさ。sizeof( object ) / sizeof( unsigned int )
	unsigned char	m_numBlocks;				// チャンクの中のブロックの数

	typedef vector<Chunk> Chunks;
	Chunks			m_chunks;					// 複数のチャンクを扱うvector
	Chunk*			m_allocChunk;				// 次に new されたときにブロックを割り当てるチャンク
    Chunk*			m_deallocChunk;				// 次に delete されたときに該当するブロックを最初に探すチャンク

public:
	FixedAllocator( size_t blockSize );
    virtual ~FixedAllocator();

	void*			Allocate();
	void			Deallocate( void* p );
};

////////////////////////////////////////////////////////////////////////////////

// チャンクの初期化
void Chunk::Init( size_t blockSize, unsigned int blocks )
{
	m_pData = new unsigned char[ blockSize * blocks ];
	m_firstAvailableBlock = 0;
	m_blocksAvailable = blocks;

	// メモリの塊を取得し、各ブロックの先頭の1byteに、自分の次のブロックのインデックスをセットしておく。これで単方向リンクリストになり、1つのブロックが消費されたら次のブロックがすぐにわかるようになっている。。
	unsigned int i = 0;
	unsigned char* p = m_pData;
	for( ; i != blocks; p += blockSize )
	{
		*p = ++i;
	}
}

// new が呼ばれたときにブロックを返す関数
void* Chunk::Allocate( size_t blockSize )
{
	// もう空きがないなら、NULLを返す。
	if( ! m_blocksAvailable )
		return NULL;

	// 次に取得可能なブロックのアドレスを pResult にセットする。
	unsigned char* pResult = m_pData + ( m_firstAvailableBlock * blockSize );

	// pResult のアドレスにセットされている、次のブロックのインデックスを m_firstAvailableBlock にセットする。
	m_firstAvailableBlock = *pResult;

	// 未使用のブロック数を1つ減らして、pResult を返す。
	--m_blocksAvailable;
	return pResult;
}

// delete が呼ばれたときに空きブロックを1つ増やす関数
void Chunk::Deallocate( void* p, size_t blockSize )
{
	// delete されたオブジェクトのアドレスがこのチャンクの先頭のアドレス以上かどうかをチェック。
	assert( p >= m_pData );

	// 空き状態に戻すブロックを取得
	unsigned char* toRelease = static_cast<unsigned char*>( p );

	// 空き状態に戻すブロックのポインタがブロックの先頭になっているかどうかのチェック。(ずれていないかのチェック)
	assert( ( toRelease - m_pData ) % blockSize == 0 );

	// 空き状態に戻すブロックに、次に new されたら返すことになっていたブロックのインデックスをセット。
	*toRelease = m_firstAvailableBlock;

	// 次に new されたら返すブロックのインデックスに、今 delete が呼ばれたブロックのインデックスをセットする。これで単方向リンクリストにdeleteされて空き領域になったブロックが追加される。
	m_firstAvailableBlock = static_cast<unsigned char>( ( toRelease - m_pData ) / blockSize );

	assert( m_firstAvailableBlock == ( toRelease - m_pData ) / blockSize );

	// 空き領域のブロック数を1つ増やす。
	++m_blocksAvailable;
}

////////////////////////////////////////////////////////////////////////////////

// blockSize には、メモリ管理するクラスのサイズ(sizeof)が来る。ブロックの数は、何個でも可。
FixedAllocator::FixedAllocator( size_t blockSize ) : m_blockSize( blockSize / sizeof( unsigned char ) ), m_numBlocks( 255 ), m_allocChunk( NULL), m_deallocChunk( NULL )
{
}

FixedAllocator::~FixedAllocator()
{
	Chunks::iterator iter = m_chunks.begin();
	Chunks::iterator end = m_chunks.end();
	for( ; iter != end; ++iter )
	{
		Chunk& chunk = *iter;
		chunk.Release();
	}
}

void* FixedAllocator::Allocate()
{
	if( m_allocChunk == NULL || m_allocChunk->m_blocksAvailable == 0 )
	{
		// このチャンクに利用可能なメモリは存在しないため、探索を開始
		Chunks::iterator i = m_chunks.begin();
		for( ; ; ++i )
		{
			if( i == m_chunks.end() )
			{
				// すべて使用中のため新たなチャンクを追加
				m_chunks.reserve( m_chunks.size() + 1 );
				Chunk newChunk;
				newChunk.Init( m_blockSize, m_numBlocks );
				m_chunks.push_back( newChunk );

				// 次にメモリの構築(new)が呼ばれたときに返すチャンクとして記憶しておく。
				m_allocChunk = &m_chunks.back();
				break;
			}
			if( i->m_blocksAvailable > 0 )
			{
				// チャンクを発見
				m_allocChunk = &*i;
				break;
			}
		}
	}

	assert( m_allocChunk != NULL );
	assert( m_allocChunk->m_blocksAvailable > 0 );
	return m_allocChunk->Allocate( m_blockSize );
}

void FixedAllocator::Deallocate( void* p )
{
    bool bFound = false;

	// m_deallocChunk は、前回解放したチャンク。同じチャンクのブロックが解放されることが多いと仮定して、まず、ここにあるかどうかを調べる。
	if( m_deallocChunk && m_deallocChunk->m_pData <= p && p <= m_deallocChunk->m_pData + m_blockSize * m_numBlocks )
    {
        m_deallocChunk->Deallocate( p, m_blockSize );
        bFound = true;
    }
    
    if( false == bFound )
    {
		// 仮定が外れたので、先頭から探していく。この処理は線形検索なので、チャンクの数が多くなればなるほど遅くなる。
        Chunks::iterator iter = m_chunks.begin();
        Chunks::iterator end = m_chunks.end();
        for( ; iter != end; ++iter )
        {
            Chunk& chunk = *iter;
            if( &chunk == m_deallocChunk )
                continue;
            
            if( chunk.m_pData <= p && p <= chunk.m_pData + m_blockSize * m_numBlocks )
            {
                chunk.Deallocate( p, m_blockSize );

				// 次回の解放は同じチャンクでしょうと仮定して、記憶しておく。
                m_deallocChunk = &chunk;
                bFound = true;
                break;
            }
        }
    }
    
    assert( bFound );
}

class CRObject
{
public:
	virtual ~CRObject(){}
    virtual int GetClassID() const = 0;
};

// FixedAllocator でメモリ管理された16 byte のクラス
class CRObject16 : public CRObject
{
	static FixedAllocator 	m_allocator;	// スタティックでメモリアロケータをメンバ変数に持つ
    double 					m_val1;			// 8byte

public:
	virtual ~CRObject16(){}
    virtual int GetClassID() const { return 1; }

	// C++ の new と delete をオーバライド。
	static void* 	operator new( size_t size ){ return m_allocator.Allocate(); }
    static void		operator delete( void* p, size_t size ){ m_allocator.Deallocate( p ); }
};

// FixedAllocator でメモリ管理された 32 byteのクラス
class CRObject32 : public CRObject
{
	static FixedAllocator 	m_allocator;	// スタティックでメモリアロケータをメンバ変数に持つ
    double 					m_val1;			// 8byte
    double 					m_val2;			// 8byte
    double 					m_val3;			// 8byte

public:
	virtual ~CRObject32(){}
    virtual int GetClassID() const { return 2; }

	// C++ の new と delete をオーバライド。
	static void* 	operator new( size_t size ){ return m_allocator.Allocate(); }
	static void		operator delete( void* p, size_t size ){ m_allocator.Deallocate( p ); }
};

// スタティック変数の初期化。プログラムが起動した直後に呼ばれます。
FixedAllocator CRObject16::m_allocator( sizeof( CRObject16 ) );
FixedAllocator CRObject32::m_allocator( sizeof( CRObject32 ) );