//
// Collection.cpp -- implementation of the class "Collection".
//
//    This file is part of the featsel program
//    Copyright (C) 2017  Marcelo S. Reis
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "Collection.h"


Collection::Collection ()
{
  // TODO
}


Collection::~Collection ()
{
  map<string, ElementSubset *>::iterator it;
  for (it = subset_collection.begin (); it != subset_collection.end (); it++)
    delete it->second;
  subset_collection.clear ();
}


bool Collection::lower_covers (ElementSubset * X)
{
  map<string, ElementSubset *>::iterator it;
  if (X == NULL)
  {
    cout << "Collection::lower_covers error: NULL subset!" << endl;
    return false;
  }
  for (it = subset_collection.begin (); it != subset_collection.end (); it++)
  {
    if (it->second->contains (X))
      return true;
  }
  return false;
}


bool Collection::upper_covers (ElementSubset * X)
{
  map<string, ElementSubset *>::iterator it;
  if (X == NULL)
  {
    cout << "Collection::upper_covers error: NULL subset!" << endl;
    return false;
  }
  for (it = subset_collection.begin (); it != subset_collection.end (); it++)
  {
    if (it->second->is_contained_by (X))
      return true;
  }
  return false;
}


unsigned int Collection::remove_lower_covered_subsets (ElementSubset * X)
{
  return remove_covered_subsets (X, true);
}


unsigned int Collection::remove_upper_covered_subsets (ElementSubset * X)
{
  return remove_covered_subsets (X, false);
}


string Collection::print_collection ()
{
  map<string, ElementSubset *>::iterator it;
  string collection_string ("");
  for (it = subset_collection.begin (); it != subset_collection.end (); it++)
    collection_string.append (it->second->print_subset ());
  return collection_string;
}


string Collection::print_collection (CostFunction * c)
{
  map<string, ElementSubset *>::iterator it;
  string collection_string ("");
  std::ostringstream value;
  for (it = subset_collection.begin (); it != subset_collection.end (); it++)
  {
    collection_string.append (it->second->print_subset ());
    collection_string.append (": ");
    value <<  c->cost (it->second);
    collection_string.append (value.str ());
    collection_string.append ("\n");
    value.str ("");
  }
  return collection_string;
}


void Collection::copy (Collection * C)
{
  map<string, ElementSubset *>::iterator it;
  for(it = C->subset_collection.begin(); it != C->subset_collection.end(); it++)
    if (! add_subset (it->second))
      cout << "Error copying collection!" << endl;
}


ElementSubset * Collection::add_subset (ElementSubset * X)
{
  pair<map<string, ElementSubset *>::iterator,bool> ret;
  ElementSubset * Y;
  Y = new ElementSubset ("", X->get_set_that_contains_this_subset());
  Y->copy (X);
  ret = subset_collection.insert
    (pair<string, ElementSubset *> (Y->print_subset (), Y));
  if (ret.second)
    return ret.first->second;
  else
  {
    delete Y;
    return NULL;
  }
}


ElementSubset * Collection::remove_last_subset ()
{
  map<string, ElementSubset *>::iterator it;
  ElementSubset * X;
  it = subset_collection.begin ();
  X = it->second;
  subset_collection.erase (it);
  return X;
}


bool Collection::has_subset (ElementSubset * X)
{
  if (subset_collection.find (X->print_subset ()) == subset_collection.end ())
    return false;
  else
    return true;
}


ElementSubset * Collection::get_subset (ElementSubset * X)
{
  map<string, ElementSubset *>::iterator it;
  it = subset_collection.find (X->print_subset ());
  if (it == subset_collection.end ())
    return NULL;
  else
    return it->second;
}


unsigned int Collection::size ()
{
  return subset_collection.size ();
}


unsigned int Collection::remove_covered_subsets
(ElementSubset * X, bool lower_cover)
{
  unsigned int number_of_removed_subsets = 0;
  map<string, ElementSubset *>::iterator it;
  for (it = subset_collection.begin (); it != subset_collection.end ();)
  {
    if ( ( ( lower_cover) && (X->contains (it->second)) ) ||
         ( (!lower_cover) && (X->is_contained_by (it->second)) ) )
    {
      delete it->second;
      subset_collection.erase (it++);
      number_of_removed_subsets++;
    }
    else it++;
  }
  return number_of_removed_subsets;
}
