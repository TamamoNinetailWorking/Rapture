#pragma once

#define ObjectPtr(_type) _type*
#define UniquePtr(_type) std::unique_ptr<_type>
#define SharedPtr(_type) std::shared_ptr<_type>
#define WeakPtr(_type) std::weak_ptr<_type>