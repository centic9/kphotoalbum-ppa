#include "BooleanGuard.h"

/**
   \class Utilities::BooleanGuard
   \brief Guard that controls entry using a static variable

   Sometimes you can get into trouble where a function calls another, which
   indirectly calls the first one again, resulting in an endless
   loop. paintEvent() and resizeEvent() are example of places where this
   sometimes happens.

   A solution is to use a static bool as a guard controlling whether the
   function is already in action. This may look like this:
   <pre>
   void f() {
      static bool guard = false;
      if ( guard )
        return;
      guard = true;
      ...
      guard = false;
   }
   </pre>
   This code of course have the problem that the guard is not correctly
   unset, if there is a premature return() in the code. This class solves
   this, and the code will look like this:

   <pre>
   void f() {
       static bool inAction = false;
       BooleanGuard guard( inAction );
       if ( !guard.canContinue() )
           return;

       ...
   }
   </pre>
**/

Utilities::BooleanGuard::BooleanGuard( bool& guard )
    : _guard( guard )
{
    if ( _guard == false ) {
        _iLocedIt =true;
        _guard = true;
    }
    else
        _iLocedIt = false;
}

Utilities::BooleanGuard::~BooleanGuard()
{
    if ( _iLocedIt )
        _guard = false;
}

bool Utilities::BooleanGuard::canContinue()
{
    return _iLocedIt;
}
