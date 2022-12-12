#ifndef THnSparseMin_H
#define THnSparseMin_H

#include <THnSparse.h>

//copied from THnSparse.cxx
class THnSparseMinCoordCompression {
public:
   THnSparseMinCoordCompression(Int_t dim, const Int_t* nbins);
   THnSparseMinCoordCompression(const THnSparseMinCoordCompression& other);
   ~THnSparseMinCoordCompression();

   THnSparseMinCoordCompression& operator=(const THnSparseMinCoordCompression& other);

   ULong64_t GetHashFromBuffer(const Char_t* buf) const;
   Int_t     GetBufferSize() const { return fCoordBufferSize; }
   Int_t     GetNdimensions() const { return fNdimensions; }
   void      SetCoordFromBuffer(const Char_t* buf_in, Int_t* coord_out) const;
   ULong64_t SetBufferFromCoord(const Int_t* coord_in, Char_t* buf_out) const;

protected:
   Int_t     GetNumBits(Int_t n) const {
      // return the number of bits allocated by the number "n"
      Int_t r = (n > 0);
      while (n/=2) ++r;
      return r;
   }
private:
   Int_t  fNdimensions;     // number of dimensions
   Int_t  fCoordBufferSize; // size of coordbuf
   Int_t *fBitOffsets;      //[fNdimensions + 1] bit offset of each axis index
};

THnSparseMinCoordCompression::THnSparseMinCoordCompression(Int_t dim, const Int_t* nbins):
   fNdimensions(dim), fCoordBufferSize(0), fBitOffsets(0)
{
   fBitOffsets = new Int_t[dim + 1];

   int shift = 0;
   for (Int_t i = 0; i < dim; ++i) {
      fBitOffsets[i] = shift;
      shift += GetNumBits(nbins[i] + 2);
   }
   fBitOffsets[dim] = shift;
   fCoordBufferSize = (shift + 7) / 8;
}

THnSparseMinCoordCompression::THnSparseMinCoordCompression(const THnSparseMinCoordCompression& other)
{
   fNdimensions = other.fNdimensions;
   fCoordBufferSize = other.fCoordBufferSize;
   fBitOffsets = new Int_t[fNdimensions + 1];
   memcpy(fBitOffsets, other.fBitOffsets, sizeof(Int_t) * fNdimensions);
}

THnSparseMinCoordCompression& THnSparseMinCoordCompression::operator=(const THnSparseMinCoordCompression& other)
{
   if (&other == this) return *this;

   fNdimensions = other.fNdimensions;
   fCoordBufferSize = other.fCoordBufferSize;
   delete [] fBitOffsets;
   fBitOffsets = new Int_t[fNdimensions + 1];
   memcpy(fBitOffsets, other.fBitOffsets, sizeof(Int_t) * fNdimensions);
   return *this;
}

THnSparseMinCoordCompression::~THnSparseMinCoordCompression()
{
   delete [] fBitOffsets;
}

void THnSparseMinCoordCompression::SetCoordFromBuffer(const Char_t* buf_in,
                                                  Int_t* coord_out) const
{
   for (Int_t i = 0; i < fNdimensions; ++i) {
      const Int_t offset = fBitOffsets[i] / 8;
      Int_t shift = fBitOffsets[i] % 8;
      Int_t nbits = fBitOffsets[i + 1] - fBitOffsets[i];
      const UChar_t* pbuf = (const UChar_t*) buf_in + offset;
      coord_out[i] = *pbuf >> shift;
      Int_t subst = (Int_t) -1;
      subst = subst << nbits;
      nbits -= (8 - shift);
      shift = 8 - shift;
      for (Int_t n = 0; n * 8 < nbits; ++n) {
         ++pbuf;
         coord_out[i] += *pbuf << shift;
         shift += 8;
      }
      coord_out[i] &= ~subst;
   }
}

ULong64_t THnSparseMinCoordCompression::SetBufferFromCoord(const Int_t* coord_in,
                                                       Char_t* buf_out) const
{
   if (fCoordBufferSize <= 8) {
      ULong64_t l64buf = 0;
      for (Int_t i = 0; i < fNdimensions; ++i) {
         l64buf += ((ULong64_t)((UInt_t)coord_in[i])) << fBitOffsets[i];
      }
      memcpy(buf_out, &l64buf, sizeof(Long64_t));
      return l64buf;
   }

   // else: doesn't fit into a Long64_t:
   memset(buf_out, 0, fCoordBufferSize);
   for (Int_t i = 0; i < fNdimensions; ++i) {
      const Int_t offset = fBitOffsets[i] / 8;
      const Int_t shift = fBitOffsets[i] % 8;
      ULong64_t val = coord_in[i];

      Char_t* pbuf = buf_out + offset;
      *pbuf += 0xff & (val << shift);
      val = val >> (8 - shift);
      while (val) {
         ++pbuf;
         *pbuf += 0xff & val;
         val = val >> 8;
      }
   }

   return GetHashFromBuffer(buf_out);
}

ULong64_t THnSparseMinCoordCompression::GetHashFromBuffer(const Char_t* buf) const
{
   // Bins are addressed in two different modes, depending
   // on whether the compact bin index fits into a Long64_t or not.
   // If it does, we can use it as a "perfect hash" for the TExMap.
   // If not we build a hash from the compact bin index, and use that
   // as the TExMap's hash.

   if (fCoordBufferSize <= 8) {
      // fits into a Long64_t
      ULong64_t hash1 = 0;
      memcpy(&hash1, buf, fCoordBufferSize);
      return hash1;
   }

   // else: doesn't fit into a Long64_t:
   ULong64_t hash = 5381;
   const Char_t* str = buf;
   while (str - buf < fCoordBufferSize) {
      hash *= 5;
      hash += *(str++);
   }
   return hash;
}

class THnSparseMinCompactBinCoord: public THnSparseMinCoordCompression {
public:
   THnSparseMinCompactBinCoord(Int_t dim, const Int_t* nbins);
   ~THnSparseMinCompactBinCoord();
   Int_t*    GetCoord() { return fCurrentBin; }
   const Char_t*   GetBuffer() const { return fCoordBuffer; }
   ULong64_t GetHash() const { return fHash; }
   void UpdateCoord() {
      fHash = SetBufferFromCoord(fCurrentBin, fCoordBuffer);
   }
   void      SetCoord(const Int_t* coord) {
      memcpy(fCurrentBin, coord, sizeof(Int_t) * GetNdimensions());
      fHash = SetBufferFromCoord(coord, fCoordBuffer);
   }
   void      SetBuffer(const Char_t* buf) {
      memcpy(fCoordBuffer, buf, GetBufferSize());
      fHash = GetHashFromBuffer(fCoordBuffer);
   }

private:
   // intentionally not implemented
   THnSparseMinCompactBinCoord(const THnSparseMinCompactBinCoord&);
   // intentionally not implemented
   THnSparseMinCompactBinCoord& operator=(const THnSparseMinCompactBinCoord&);

private:
   ULong64_t fHash;      // hash for current coordinates; 0 if not calculated
   Char_t *fCoordBuffer; // compact buffer of coordinates
   Int_t *fCurrentBin;   // current coordinates
};

THnSparseMinCompactBinCoord::THnSparseMinCompactBinCoord(Int_t dim, const Int_t* nbins):
   THnSparseMinCoordCompression(dim, nbins),
   fHash(0), fCoordBuffer(0), fCurrentBin(0)
{
   fCurrentBin = new Int_t[dim];
   size_t bufAllocSize = GetBufferSize();
   if (bufAllocSize < sizeof(Long64_t))
      bufAllocSize = sizeof(Long64_t);
   fCoordBuffer = new Char_t[bufAllocSize];
}

THnSparseMinCompactBinCoord::~THnSparseMinCompactBinCoord()
{
   delete [] fCoordBuffer;
   delete [] fCurrentBin;
}

//avoid over-allocating for hash map
class THnSparseMin : public THnBase {
 private:
   //copied from THnSparse.h
   Int_t      fChunkSize;    // number of entries for each chunk
   Long64_t   fFilledBins;   // number of filled bins
   TObjArray  fBinContent;   // array of THnSparseArrayChunk
   TExMap     fBins;         //! filled bins
   TExMap     fBinsContinued;//! filled bins for non-unique hashes, containing pairs of (bin index 0, bin index 1)
   THnSparseMinCompactBinCoord *fCompactCoord; //! compact coordinate
 public:
   Long64_t GetNbins() const { return fFilledBins; }
   //copied from THnSparse.cxx
   THnSparseMinCompactBinCoord* GetCompactCoord() const
   {
      if (!fCompactCoord) {
         Int_t *bins = new Int_t[fNdimensions];
         for (Int_t d = 0; d < fNdimensions; ++d)
            bins[d] = GetAxis(d)->GetNbins();
         const_cast<THnSparseMin*>(this)->fCompactCoord
            = new THnSparseMinCompactBinCoord(fNdimensions, bins);
         delete [] bins;
      }
      return fCompactCoord;
   }
   //changed capacity/expand lines
   //check if already allocated before proceeding
   void FillExMap()
   {
      if(fBins.GetSize()) return;
      TIter iChunk(&fBinContent);
      THnSparseArrayChunk* chunk = 0;
      THnSparseMinCoordCompression compactCoord(*GetCompactCoord());
      Long64_t idx = 0;
      if (GetNbins() > fBins.Capacity())
         fBins.Expand(GetNbins());
      while ((chunk = (THnSparseArrayChunk*) iChunk())) {
         const Int_t chunkSize = chunk->GetEntries();
         Char_t* buf = chunk->fCoordinates;
         const Int_t singleCoordSize = chunk->fSingleCoordinateSize;
         const Char_t* endbuf = buf + singleCoordSize * chunkSize;
         for (; buf < endbuf; buf += singleCoordSize, ++idx) {
            Long64_t hash = compactCoord.GetHashFromBuffer(buf);
            Long64_t linidx = fBins.GetValue(hash);
            if (linidx) {
               Long64_t nextidx = linidx;
               while (nextidx) {
                  // must be a collision, so go to fBinsContinued.
                  linidx = nextidx;
                  nextidx = fBinsContinued.GetValue(linidx);
               }
               fBinsContinued.Add(linidx, idx + 1);
            } else {
               fBins.Add(hash, idx + 1);
            }
         }
      }
   }
};

#endif
