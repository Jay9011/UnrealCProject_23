#pragma once

#include "CoreMinimal.h"

#define CheckTrue(x) { if(x == true) return; };
#define CheckTrueResult(x, y) { if(x == true) return y; };

#define CheckFalse(x) { if (x == false) return; };
#define CheckFalseResult(x, y) { if(x == false) return y;};

#define CheckNull(x) { if(x == nullptr) return;};
#define CheckNullResult(x, y) { if(x == nullptr) return y;};
