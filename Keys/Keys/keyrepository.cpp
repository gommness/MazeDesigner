#include "keyrepository.h"

KeyRepository &operator<<(KeyRepository &repo, const Key &val)
{
    repo.append(val);
    return repo;
}
