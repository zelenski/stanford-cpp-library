/*
 * File: lexicon.h
 * ---------------
 * This file exports the <code>Lexicon</code> class, which
 * is a typedef you can set to either TrieLexicon or DawgLexicon.
 * As of Fall 2023, Lexicon defaults to TrieLexicon.
 */

#pragma once

#include "trielexicon.h"
#include "dawglexicon.h"

typedef TrieLexicon Lexicon;
