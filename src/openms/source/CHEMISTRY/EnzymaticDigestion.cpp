// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2015.
//
// This software is released under a three-clause BSD license:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of any author or any participating institution
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// For a full list of authors, refer to the file AUTHORS.
// --------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL ANY OF THE AUTHORS OR THE CONTRIBUTING
// INSTITUTIONS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// --------------------------------------------------------------------------
// $Maintainer: Chris Bielow, Xiao Liang $
// $Authors: Marc Sturm, Chris Bielow$
// --------------------------------------------------------------------------

#include <OpenMS/CHEMISTRY/EnzymaticDigestion.h>
#include <OpenMS/CHEMISTRY/EnzymesDB.h>
#include <OpenMS/SYSTEM/File.h>
#include <OpenMS/CONCEPT/LogStream.h>
#include <boost/regex.hpp>

#include <iostream>

using namespace std;

namespace OpenMS
{
  const std::string EnzymaticDigestion::NamesOfSpecificity[] = {"full", "semi", "none"};


  EnzymaticDigestion::EnzymaticDigestion() :
    missed_cleavages_(0),
    enzyme_(*EnzymesDB::getInstance()->getEnzyme("Trypsin")),
    specificity_(SPEC_FULL)
  {
  }

  EnzymaticDigestion::EnzymaticDigestion(const EnzymaticDigestion& rhs) :
    missed_cleavages_(rhs.missed_cleavages_),
    enzyme_(rhs.enzyme_),
    specificity_(rhs.specificity_)
  {
  }

  /// Assignment operator
  EnzymaticDigestion& EnzymaticDigestion::operator=(const EnzymaticDigestion& rhs)
  {
    if (this != &rhs)
    {
      missed_cleavages_ = rhs.missed_cleavages_;
      enzyme_ = rhs.enzyme_;
      specificity_ = rhs.specificity_;
    }
    return *this;
  }

  SignedSize EnzymaticDigestion::getMissedCleavages() const
  {
    return missed_cleavages_;
  }

  void EnzymaticDigestion::setMissedCleavages(SignedSize missed_cleavages)
  {
    missed_cleavages_ = missed_cleavages;
  }

  void EnzymaticDigestion::setEnzyme(const String enzyme_name)
  {
    enzyme_ = *EnzymesDB::getInstance()->getEnzyme(enzyme_name);
  }

  String EnzymaticDigestion::getEnzymeName() const
  {
    return enzyme_.getName();
  }

  EnzymaticDigestion::Specificity EnzymaticDigestion::getSpecificityByName(const String& name)
  {
    for (Size i = 0; i < SIZE_OF_SPECIFICITY; ++i)
    {
      if (name == NamesOfSpecificity[i]) return Specificity(i);
    }
    return SIZE_OF_SPECIFICITY;
  }

  EnzymaticDigestion::Specificity EnzymaticDigestion::getSpecificity() const
  {
    return specificity_;
  }

  void EnzymaticDigestion::setSpecificity(Specificity spec)
  {
    specificity_ = spec;
  }

  std::vector<Size> EnzymaticDigestion::tokenize_(const String& s) const
  {
    std::vector<Size> pep_positions;
    Size pos = 0;
    boost::regex re(enzyme_.getRegEx());
    if (enzyme_.getRegEx() != "()") // if it's not "no cleavage"
    {
      boost::sregex_token_iterator i(s.begin(), s.end(), re, -1);
      boost::sregex_token_iterator j;
      while (i != j)
      {
        pep_positions.push_back(pos);
        pos += (i++)->length();
      }
<<<<<<< HEAD
=======
      break;

    case ENZYME_TRYPSIN_P:
      if (use_log_model_)
      {
        throw Exception::InvalidParameter(__FILE__, __LINE__, __PRETTY_FUNCTION__, String("EnzymaticDigestion: enzyme '") + NamesOfEnzymes[ENZYME_TRYPSIN_P] + " does not support logModel!");
      }
      else
      {
        // R or K at the end,  presence of P does not matter
        return *iterator == 'R' || *iterator == 'K';
      }
      break;

    default:
      return false;
>>>>>>> [NOP] style fixes
    }
  }

  bool EnzymaticDigestion::isCleavageSiteInString_(const String& protein, const String::const_iterator& iterator) const
  {
    switch (enzyme_)
    {
    case ENZYME_TRYPSIN:
      if (use_log_model_)
      {
        //TODO: throw not implemented exception
      }
      else // naive digestion
      {
        // R or K at the end and not P afterwards
        return (*iterator == 'R' || *iterator == 'K') && ((iterator + 1) == protein.end() || *(iterator + 1) != 'P');
      }

    default:
      return false;
    }
  }

  void EnzymaticDigestion::nextCleavageSite_(const AASequence& protein, AASequence::ConstIterator& iterator) const
  {
    while (iterator != protein.end())
    {
      pep_positions.push_back(pos);
    }
    return pep_positions;
  }

  void EnzymaticDigestion::nextCleavageSiteInString_(const String& sequence, String::const_iterator& iterator) const
  {
    while (iterator != sequence.end())
    {
      if (isCleavageSiteInString_(sequence, iterator))
      {
        ++iterator;
        return;
      }
      ++iterator;
    }
    return;
  }

  bool EnzymaticDigestion::isValidProduct(const AASequence& protein, Size pep_pos, Size pep_length)
  {
    if (pep_pos >= protein.size())
    {
      LOG_WARN << "Error: start of peptide is beyond end of protein!" << endl;
      return false;
    }
    else if (pep_pos + pep_length > protein.size())
    {
      LOG_WARN << "Error: end of peptide is beyond end of protein!" << endl;
      return false;
    }
    else if (pep_length == 0 || protein.size() == 0)
    {
      LOG_WARN << "Error: peptide or protein must not be empty!" << endl;
      return false;
    }

    if (specificity_ == SPEC_NONE)
    {
      return true; // we don't care about terminal ends
    }
    else // either SPEC_SEMI or SPEC_FULL
    {
      bool spec_c = false, spec_n = false;

      std::vector<Size> pep_positions = tokenize_(protein.toUnmodifiedString());
      // test each end
      if (pep_pos == 0 ||
          std::find(pep_positions.begin(), pep_positions.end(), pep_pos) != pep_positions.end())
      {
        spec_n = true;
      }
      // if allow methionine cleavage at the protein start position
      if (pep_pos == 1 && methionine_cleavage && protein.getResidue((Size)0).getOneLetterCode() == "M")
      {
        spec_n = true;
      }
      if (pep_pos + pep_length == protein.size() ||
          std::find(pep_positions.begin(), pep_positions.end(), pep_pos  + pep_length) != pep_positions.end())
      {
        spec_c = true;
      }

      if (spec_n && spec_c)
      {
        return true; // if both are fine, its definitely valid
      }
      else if ((specificity_ == SPEC_SEMI) && (spec_n || spec_c))
      {
        return true; // one only for SEMI
      }
      else
      {
        return false;
      }
    }
  }

  Size EnzymaticDigestion::peptideCount(const AASequence& protein)
  {
    std::vector<Size> pep_positions = tokenize_(protein.toUnmodifiedString());
    SignedSize count = pep_positions.size();
    // missed cleavages
    Size sum = count;
    for (SignedSize i = 1; i < count; ++i)
    {
      if (i > missed_cleavages_) break;
      sum += count - i;
    }
    return sum;
  }

  void EnzymaticDigestion::digest(const AASequence& protein, vector<AASequence>& output) const
  {
    // initialization
    output.clear();
    // naive cleavage sites
    Size missed_cleavages = missed_cleavages_;
    std::vector<Size> pep_positions = tokenize_(protein.toUnmodifiedString());
    Size count = pep_positions.size();
    Size begin = pep_positions[0];
    for (Size i = 1; i < count; ++i)
    {
      output.push_back(protein.getSubsequence(begin, pep_positions[i] - begin));
      begin = pep_positions[i];
    }
    output.push_back(protein.getSubsequence(begin, protein.size() - begin));

    //missed cleavages
    if (pep_positions.size() > 0 && missed_cleavages_ != 0) //there is at least one cleavage site!
    {
      //generate fragments with missed cleavages
      for (Size i = 1; ((i <= missed_cleavages) && (count > i)); ++i)
      {
        begin = pep_positions[0];
        for (Size j = 1; j < count - i; ++j)
        {
          output.push_back(protein.getSubsequence(begin, pep_positions[j + i] - begin));
          begin = pep_positions[j];
        }
        output.push_back(protein.getSubsequence(begin, protein.size() - begin));
      }
    }
  }

////  void EnzymaticDigestion::digestUnmodifiedString(const String& sequence, vector<pair<String::const_iterator, String::const_iterator> >& output, Size min_length = 0) const
//  {
//    // initialization
//    SignedSize count = 1;
//    output.clear();

//    SignedSize missed_cleavages = missed_cleavages_;

//    // missed cleavage iterators
//    vector<String::ConstIterator> mc_iterators;

//    if (missed_cleavages != 0)
//    {
//      mc_iterators.push_back(sequence.begin());
//    }

//    String::const_iterator begin = sequence.begin();
//    String::const_iterator end = sequence.begin();
//    while (nextCleavageSiteInString_(sequence, end), end != sequence.end())
//    {
//      ++count;
//      if (missed_cleavages != 0)
//      {
//        mc_iterators.push_back(end);
//      }
//      // store begin and (one after) end position of subsequence
//      if (static_cast<Size>(end - begin) >= min_length)
//        output.push_back(make_pair(begin, end));
//      begin = end;
//    }
//    // add last sequence
//    if (static_cast<Size>(end - begin) >= min_length)
//      output.push_back(make_pair(begin, end));

//    if (missed_cleavages != 0)
//    {
//      mc_iterators.push_back(end);
//    }

//    // missed cleavages
//    if (mc_iterators.size() > 2) //there is at least one cleavage site!
//    {
//      // resize to number of fragments
//      Size sum = count;

//      for (SignedSize i = 1; i < count; ++i)
//      {
//        if (i > missed_cleavages_)
//        {
//          break;
//        }
//        sum += count - i;
//      }

//      // generate fragments with missed cleavages
//      for (SignedSize i = 1; ((i <= missed_cleavages_) && (count > i)); ++i)
//      {
//        vector<String::const_iterator>::const_iterator b = mc_iterators.begin();
//        vector<String::const_iterator>::const_iterator e = b + (i + 1);
//        while (e != mc_iterators.end())
//        {
//          if (static_cast<Size>(*e - *b) >= min_length)
//            output.push_back(make_pair(*b, *e));
//          ++b;
//          ++e;
//        }
//      }
//    }
//  }

} //namespace
