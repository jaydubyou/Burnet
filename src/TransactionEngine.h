#ifndef __TRANSACTIONENGINE__
#define __TRANSACTIONENGINE__

#include "Ledger.h"
#include "Ledger.h"
#include "Currency.h"
#include "SerializedTransaction.h"
#include "SerializedLedger.h"

// A TransactionEngine applies serialized transactions to a ledger
// It can also, verify signatures, verify fees, and give rejection reasons

enum TransactionEngineResult
{ // <0 = Can never succeed, 0 = success, >0 = failed, but could succeed
	terUNKNOWN     = -3,	// The transactions requires logic not implemented yet
	terINSUF_FEE_P = -2,	// fee totally insufficient
	terINVALID     = -1,	// The transaction is ill-formed
	terSUCCESS     = 0,		// The transaction was applied
	terALREADY,				// The transaction was already in the ledger
	terINSUF_FEE_T,			// fee insufficient now (account doesn't exist, network load)
	terUNFUNDED,			// Source account had insufficient balance
	terNO_PATH,				// No path existed or met transaction/balance requirements
	terPAST_SEQ,			// This sequence number has already past
	terPRE_SEQ,				// Missing/inapplicable prior transaction
	terPAST_LEDGER,			// The transaction expired and can't be applied
};

enum TransactionEngineParams
{
	tepNO_CHECK_SIGN = 1,	// Signature already checked
	tepNO_CHECK_FEE  = 2,	// It was voted into a ledger anyway
};

class TransactionEngine
{
protected:
	Ledger::pointer mLedger;

	TransactionEngineResult doPayment(const SerializedTransaction&, SerializedLedgerEntry& source);
	TransactionEngineResult doInvoice(const SerializedTransaction&, SerializedLedgerEntry& source);
	TransactionEngineResult doOffer(const SerializedTransaction&, SerializedLedgerEntry& source);
	TransactionEngineResult doTake(const SerializedTransaction&, SerializedLedgerEntry& source);
	TransactionEngineResult doCancel(const SerializedTransaction&, SerializedLedgerEntry& source);
	TransactionEngineResult doStore(const SerializedTransaction&, SerializedLedgerEntry& source);
	TransactionEngineResult doDelete(const SerializedTransaction&, SerializedLedgerEntry& source);

public:
	TransactionEngine() { ; }
	TransactionEngine(Ledger::pointer ledger) : mLedger(ledger) { ; }

	Ledger::pointer getLedger() { return mLedger; }
	void setLedger(Ledger::pointer ledger) { mLedger = ledger; }

	TransactionEngineResult applyTransaction(const SerializedTransaction&, TransactionEngineParams);
};

#endif
