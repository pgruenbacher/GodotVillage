/*******************************************************************************
 *
 *     Copyright : 2010 Arnaud Emilien
 *
 *******************************************************************************/

#ifndef SCENARIO_H
#define SCENARIO_H

#include "Sequence.h"
#include "../Utils/LinkedList.h"

class Scenario{
private :
        LinkedList<Sequence> _sequences;
public :
        Scenario(){}
        void addSequence(const Sequence& sequence)
        {
                _sequences.pushLast(sequence);
        }
        LinkedList<Sequence> getSequences() const { return _sequences; }

}; // class Scenario

#endif // SCENARIO_H
