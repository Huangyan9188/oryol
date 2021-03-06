//------------------------------------------------------------------------------
//  schemeRegistry.cc
//------------------------------------------------------------------------------
#include "Pre.h"
#include "schemeRegistry.h"
#include "IO/FS/FileSystem.h"

namespace Oryol {
namespace _priv {

//------------------------------------------------------------------------------
schemeRegistry::schemeRegistry() {
    // empty
}

//------------------------------------------------------------------------------
schemeRegistry::~schemeRegistry() {
    // empty
}

//------------------------------------------------------------------------------
void
schemeRegistry::RegisterFileSystem(const StringAtom& scheme, std::function<Ptr<FileSystem>()> fsCreator) {
    this->rwLock.LockWrite();
    o_assert(!this->registry.Contains(scheme));
    this->registry.Add(scheme, fsCreator);
    this->rwLock.UnlockWrite();
}

//------------------------------------------------------------------------------
void
schemeRegistry::UnregisterFileSystem(const StringAtom& scheme) {
    this->rwLock.LockWrite();
    o_assert(this->registry.Contains(scheme));
    this->registry.Erase(scheme);
    this->rwLock.UnlockWrite();
}

//------------------------------------------------------------------------------
bool
schemeRegistry::IsFileSystemRegistered(const StringAtom& scheme) const {
    this->rwLock.LockRead();
    bool result = this->registry.Contains(scheme);
    this->rwLock.UnlockRead();
    return result;
}

//------------------------------------------------------------------------------
Ptr<FileSystem>
schemeRegistry::CreateFileSystem(const StringAtom& scheme) const {
    this->rwLock.LockRead();
    o_assert(this->registry.Contains(scheme));
    Ptr<FileSystem> fileSystem(this->registry[scheme]());
    this->rwLock.UnlockRead();
    return fileSystem;
}

} // namespace _priv
} // namespace Oryol