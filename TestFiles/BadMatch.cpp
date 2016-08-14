/*
    {- match is wrong because of "}".
*/

    if (isQuoted && tmp.equals(closeBracket))
    {
     tokens.add(tmpResult);

     if (tmp.equals("\"}"))
      tokens.add(tmp);

     isQuoted = false;
     continue;
    }


